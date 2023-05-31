#include "../../thirdparty/json.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <ctime>
#include <atomic>

#include <unistd.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../../include/public.hpp"
#include "../../include/server/model/user.hpp"
#include "../../include/server/model/group.hpp"

using namespace std;
using json = nlohmann::json;

// 当前用户
User c_curUser;
// 当前用户好友列表
vector<User> c_curFriendList;
// 当前用户群组列表
vector<Group> c_curGroupList;

// 控制主菜单页面程序
bool c_isMainMenuRunning = false;
// 用于读写线程之间的通信
sem_t rw_sem;
// 记录登录状态
atomic_bool c_isLoginSuccess{false};

// 显示当前用户信息
void showCurInfo();
// 接收线程，发送和接受应该是并行的
void readHandler(int clientfd);
// 获取当前时间
string getCurTime();
// 主菜单
void mainMenu(int);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    // 创建client端的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        cerr << "socket create error" << endl;
        exit(-1);
    }

    // 填写client需要连接的server信息ip+port
    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    // client和server进行连接
    if (-1 == connect(clientfd, (sockaddr *)&server, sizeof(sockaddr_in)))
    {
        cerr << "connect server error" << endl;
        close(clientfd);
        exit(-1);
    }

    // 连接服务器成功，启动接收子线程
    std::thread readTask(readHandler, clientfd);
    // 设置线程分离
    readTask.detach();

    // main线程用于接收用户输入，负责发送数据
    for (;;)
    {
        // 显示首页面菜单 登录、注册、退出
        cout << "========================" << endl;
        cout << "1. login" << endl;
        cout << "2. register" << endl;
        cout << "3. quit" << endl;
        cout << "========================" << endl;
        cout << "choice:";
        int choice = 0;
        cin >> choice;
        cin.get(); // 处理缓冲区残留的回车

        switch (choice)
        {
        case 1: // login业务
        {
            int id = 0;
            char pwd[50] = {0};
            cout << "userid:";
            cin >> id;
            cin.get(); // 处理缓冲区残留的回车
            cout << "userpassword:";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = LOGIN_MSG;
            js["id"] = id;
            js["password"] = pwd;
            string request = js.dump();

            c_isLoginSuccess = false;

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (len == -1)
            {
                cerr << "send login msg error:" << request << endl;
            }

            sem_wait(&rw_sem); // 等待信号量，由子线程处理完登录的响应消息后，通知这里

            if (c_isLoginSuccess)
            {
                // 进入聊天主菜单页面
                c_isMainMenuRunning = true;
                mainMenu(clientfd);
            }
            break;
        }
        case 2: // register业务
        {
            char name[50] = {0};
            char pwd[50] = {0};
            cout << "username:";
            cin.getline(name, 50); // 用getline是为了避免name里有空格
            cout << "userpassword:";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = REG_MSG;
            js["name"] = name;
            js["password"] = pwd;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (len == -1)
            {
                cerr << "send reg msg error:" << request << endl;
            }

            sem_wait(&rw_sem); // 等待信号量，子线程处理完注册消息会通知
            break;
        }
        case 3: // quit业务
        {
            close(clientfd);
            sem_destroy(&rw_sem);
            exit(0);
        }
        default:
            cerr << "invalid input!" << endl;
            break;
        }
    }

    return 0;
}

void doRegResponse(json &js)
{
    if (0 != js["errno"].get<int>()) // 注册失败
    {
        cerr << "name is already exist, register error!" << endl;
    }
    else // 注册成功
    {
        cout << "name register success, userid is " << js["id"]
             << ", do not forget it!" << endl;
    }
}
void doLoginResponse(json &js)
{
    if (0 != js["errno"].get<int>()) // 登录失败
    {
        cerr << js["errmsg"] << endl;
        c_isLoginSuccess = false;
    }
    else // 登录成功
    {
        // 记录当前用户的id和name
        c_curUser.setId(js["id"].get<int>());
        c_curUser.setName(js["name"]);

        // 记录当前用户的好友列表信息
        if (js.contains("friends"))
        {
            // 初始化
            c_curFriendList.clear();

            vector<string> vec = js["friends"];
            for (string &str : vec)
            {
                json js = json::parse(str);
                User user;
                user.setId(js["id"].get<int>());
                user.setName(js["name"]);
                user.setState(js["state"]);
                c_curFriendList.push_back(user);
            }
        }

        // 记录当前用户的群组列表信息
        if (js.contains("groups"))
        {
            // 初始化
            c_curGroupList.clear();

            vector<string> vec1 = js["groups"];
            for (string &groupstr : vec1)
            {
                json grpjs = json::parse(groupstr);
                Group group;
                group.setId(grpjs["id"].get<int>());
                group.setName(grpjs["groupname"]);
                group.setDesc(grpjs["groupdesc"]);

                vector<string> vec2 = grpjs["users"];
                for (string &userstr : vec2)
                {
                    GroupUser user;
                    json js = json::parse(userstr);
                    user.setId(js["id"].get<int>());
                    user.setName(js["name"]);
                    user.setState(js["state"]);
                    user.setRole(js["role"]);
                    group.getUsers().push_back(user);
                }

                c_curGroupList.push_back(group);
            }
        }

        // 显示登录用户的基本信息
        showCurInfo();

        // 显示当前用户的离线消息  个人聊天信息或者群组消息
        if (js.contains("offlinemsg"))
        {
            vector<string> vec = js["offlinemsg"];
            for (string &str : vec)
            {
                json js = json::parse(str);
                // time + [id] + name + " said: " + xxx
                if (ONE_CHAT_MSG == js["msgid"].get<int>())
                {
                    cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                         << " said: " << js["msg"].get<string>() << endl;
                }
                else
                {
                    cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                         << " said: " << js["msg"].get<string>() << endl;
                }
            }
        }

        c_isLoginSuccess = true;
    }
}

// 子线程 - 接收线程
void readHandler(int clientfd)
{
    for (;;)
    {
        char buffer[1024] = {0};
        int len = recv(clientfd, buffer, 1024, 0); // 阻塞了
        if (-1 == len || 0 == len)
        {
            close(clientfd);
            exit(-1);
        }

        // 接收ChatServer转发的数据，反序列化生成json数据对象
        json js = json::parse(buffer);
        int msgtype = js["msgid"].get<int>();

        switch (msgtype)
        {
        case ONE_CHAT_MSG:
            cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                 << " said: " << js["msg"].get<string>() << endl;
            break;
        case GROUP_CHAT_MSG:
            cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                 << " said: " << js["msg"].get<string>() << endl;
            break;
        case LOGIN_MSG_ACK:
            doLoginResponse(js); // 处理登录响应的业务逻辑
            sem_post(&rw_sem);   // 通知主线程，登录结果处理完成
            break;
        case REG_MSG_ACK:
            doRegResponse(js);
            sem_post(&rw_sem); // 通知主线程，注册结果处理完成
            break;
        default:
            break;
        }

        // if (ONE_CHAT_MSG == msgtype)
        // {
        //     cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
        //          << " said: " << js["msg"].get<string>() << endl;
        //     continue;
        // }

        // if (GROUP_CHAT_MSG == msgtype)
        // {
        //     cout << "群消息[" << js["groupid"] << "]:" << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
        //          << " said: " << js["msg"].get<string>() << endl;
        //     continue;
        // }

        // if (LOGIN_MSG_ACK == msgtype)
        // {
        //     cout << "333" << endl;
        //     doLoginResponse(js); // 处理登录响应的业务逻辑
        //     sem_post(&rw_sem);   // 通知主线程，登录结果处理完成
        //     continue;
        // }

        // if (REG_MSG_ACK == msgtype)
        // {
        //     doRegResponse(js);
        //     sem_post(&rw_sem); // 通知主线程，注册结果处理完成
        //     continue;
        // }
    }
}

void showCurInfo()
{
    cout << "======================login user======================" << endl;
    cout << "current login user => id:" << c_curUser.getId() << " name:" << c_curUser.getName() << endl;
    cout << "----------------------friend list---------------------" << endl;
    if (!c_curFriendList.empty())
    {
        for (User &user : c_curFriendList)
        {
            cout << user.getId() << " " << user.getName() << " " << user.getState() << endl;
        }
    }
    cout << "----------------------group list----------------------" << endl;
    if (!c_curGroupList.empty())
    {
        for (Group &group : c_curGroupList)
        {
            cout << group.getId() << " " << group.getName() << " " << group.getDesc() << endl;
            for (GroupUser &user : group.getUsers())
            {
                cout << " " << user.getId() << " " << user.getName() << " " << user.getState()
                     << " " << user.getRole() << endl;
            }
        }
    }
    cout << "======================================================" << endl;
}

// "help" command handler
void help(int clientfd = 0, string str = "");
// "chat" command handler
void chat(int clientfd, string str);
// "addfriend" command handler
void addfriend(int clientfd, string str);
// "creategroup" command handler
void creategroup(int clientfd, string str);
// "addgroup" command handler
void addgroup(int clientfd, string str);
// "groupchat" command handler
void groupchat(int clientfd, string str);
// "loginout" command handler
void loginout(int clientfd, string str);
// "showstate" command handler
void showstate(int clientfd, string str);

// 系统支持的客户端命令列表
unordered_map<string, string> commandMap = {
    {"help", "显示所有支持的命令，格式help"},
    {"chat", "一对一聊天，格式chat:friendid:message"},
    {"addfriend", "添加好友，格式addfriend:friendid"},
    {"creategroup", "创建群组，格式creategroup:groupname:groupdesc"},
    {"addgroup", "加入群组，格式addgroup:groupid"},
    {"groupchat", "群聊，格式groupchat:groupid:message"},
    {"loginout", "注销，格式loginout"},
    {"showstate", "刷新当前好友信息等状态"}
};

// 注册系统支持的客户端命令处理
unordered_map<string, function<void(int, string)>> commandHandlerMap = {
    {"help", help},
    {"chat", chat},
    {"addfriend", addfriend},
    {"creategroup", creategroup},
    {"addgroup", addgroup},
    {"groupchat", groupchat},
    {"loginout", loginout},
    {"showstate", showstate}
};

// 主菜单
void mainMenu(int clientfd)
{
    help();

    char buffer[1024] = {0};
    while (c_isMainMenuRunning)
    {
        cin.getline(buffer, 1024);
        string commandbuf(buffer);
        string command; // 存储命令
        int idx = commandbuf.find(":");
        if (-1 == idx)
        {
            command = commandbuf;
        }
        else
        {
            command = commandbuf.substr(0, idx);
        }
        auto it = commandHandlerMap.find(command);
        if (it == commandHandlerMap.end())
        {
            cerr << "invalid input command!" << endl;
            continue;
        }

        // 调用相应命令的事件处理回调，mainMenu对修改封闭，添加新功能不需要修改该函数
        it->second(clientfd, commandbuf.substr(idx + 1, commandbuf.size() - idx)); // 调用命令处理方法
    }
}

// "help" command handler
void help(int clientfd, string str)
{
    cout << "show command list >>> " << endl;
    for (auto &p : commandMap)
    {
        cout << p.first << " : " << p.second << endl;
    }
    cout << endl;
}
// "chat" command handler
void chat(int clientfd, string str)
{
    int idx = str.find(":"); // friendid:message
    if (-1 == idx)
    {
        cerr << "chat command invalid!" << endl;
        return;
    }

    int friendid = atoi(str.substr(0, idx).c_str());
    string message = str.substr(idx + 1, str.size() - idx);
    // 组装json
    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = c_curUser.getId();
    js["name"] = c_curUser.getName();
    js["to"] = friendid;
    js["msg"] = message;
    js["time"] = getCurTime();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send chat msg error -> " << buffer << endl;
    }
}
// "addfriend" command handler
void addfriend(int clientfd, string str)
{
    // friendid
    int id = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = c_curUser.getId();
    js["friendid"] = id;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "add friend msg error ->" << buffer << endl;
    }
}
// "creategroup" command handler
void creategroup(int clientfd, string str)
{
    int idx = str.find(":");
    if (-1 == idx)
    {
        cerr << "creategroup command invalid!" << endl;
        return;
    }

    string groupname = str.substr(0, idx);
    string groupdesc = str.substr(idx + 1, str.size() - idx);

    json js;
    js["msgid"] = CREATE_GROUP_MSG;
    js["id"] = c_curUser.getId();
    js["groupname"] = groupname;
    js["groupdesc"] = groupdesc;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send creategroup msg error -> " << buffer << endl;
    }
}
// "addgroup" command handler
void addgroup(int clientfd, string str)
{
    int groupid = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = c_curUser.getId();
    js["groupid"] = groupid;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send addgroup msg error -> " << buffer << endl;
    }
}
// "groupchat" command handler
void groupchat(int clientfd, string str)
{
    int idx = str.find(":");
    if (-1 == idx)
    {
        cerr << "groupchat command invalid!" << endl;
        return;
    }

    int groupid = atoi(str.substr(0, idx).c_str());
    string message = str.substr(idx + 1, str.size() - idx);

    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = c_curUser.getId();
    js["name"] = c_curUser.getName();
    js["groupid"] = groupid;
    js["msg"] = message;
    js["time"] = getCurTime();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send groupchat msg error -> " << buffer << endl;
    }
}
// "loginout" command handler
void loginout(int clientfd, string str)
{
    json js;
    js["msgid"] = LOGINOUT_MSG;
    js["id"] = c_curUser.getId();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send loginout msg error -> " << buffer << endl;
    }
    else
    {
        c_isMainMenuRunning = false;
    }
}
// "showstate" command handler
void showstate(int clientfd, string str)
{
    showCurInfo();
}

// 获取系统时间（聊天信息需要添加时间信息）
string getCurTime()
{
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
            (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
            (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}

