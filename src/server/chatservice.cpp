#include "../../include/server/chatservice.hpp"
#include "../../include/public.hpp"

// moduo库的日志
#include <muduo/base/Logging.h>

#include <iostream>

ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

ChatService::ChatService()
{
    m_msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    m_msgHandlerMap.insert({LOGINOUT_MSG, std::bind(&ChatService::loginOut, this, _1, _2, _3)});
    m_msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    m_msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    m_msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});

    m_msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    m_msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    m_msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3)});

    if (m_redis.connect())
    {
        m_redis.init_notify_handler(std::bind(&ChatService::redisHandler, this, _1, _2));
    }
}

void ChatService::reset()
{
    // 把online状态的用户设置为offline
    m_userModel.resetState();
}

MsgHandler ChatService::getHandler(int msgid)
{
    auto it = m_msgHandlerMap.find(msgid);
    if (it == m_msgHandlerMap.end())
    {
        // 返回一个默认空操作的handler，用lambda表达式
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time)
        {
            LOG_ERROR << "msgid:" << msgid << " can not find handler!";
        };
    }
    else
    {
        return m_msgHandlerMap[msgid];
    }
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = m_userModel.query(id);
    if (user.getId() == id && user.getPassword() == pwd)
    {
        if (user.getState() == "online")
        {
            // 已登录，不允许重复登陆
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "该用户已登录！";
            conn->send(response.dump());
        }
        else
        {
            // success
            // 保存连接信息，注意线程安全问题
            {
                lock_guard<mutex> lock(m_connMutex);
                m_userConnMap.insert({id, conn});
            }

            // id用户登录成功后，向redis订阅channel(id)
            m_redis.subscribe(id);

            // 更新用户信息 state offline -> online
            user.setState("online");
            m_userModel.updateState(user);
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();

            // 查询用户是否有离线消息
            vector<string> vec = m_offlineMsgModel.query(id);
            if (!vec.empty())
            {
                response["offlinemsg"] = vec;
                // 读取后要删除掉该消息
                m_offlineMsgModel.remove(id);
            }
            // 查询该用户的好友信息并返回
            vector<User> userVec = m_friendModel.query(id);
            if (!userVec.empty())
            {
                vector<string> vec2;
                for (User &user : userVec)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            // 查询用户的群组信息
            vector<Group> groupuserVec = m_groupModel.queryGroups(id);
            if (!groupuserVec.empty())
            {
                // group:[{groupid:[xxx, xxx, xxx, xxx]}]
                vector<string> groupV;
                for (Group &group : groupuserVec)
                {
                    json grpjson;
                    grpjson["id"] = group.getId();
                    grpjson["groupname"] = group.getName();
                    grpjson["groupdesc"] = group.getDesc();
                    vector<string> userV;
                    for (GroupUser &user : group.getUsers())
                    {
                        json js;
                        js["id"] = user.getId();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    grpjson["users"] = userV;
                    groupV.push_back(grpjson.dump());
                }

                response["groups"] = groupV;
            }

            conn->send(response.dump());
        }
        // {"msgid":1, "id":1, "password":"123456"}
    }
    else
    {
        // fail
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "用户名或密码不正确！";
        conn->send(response.dump());
    }
}

void ChatService::loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();

    {
        lock_guard<mutex> lock(m_connMutex);
        auto it = m_userConnMap.find(userid);
        if (it != m_userConnMap.end())
        {
            m_userConnMap.erase(it);
        }
    }

    // 用户注销，相当于就是下线，在redis中取消订阅通道
    m_redis.unsubscribe(userid);

    // 更新用户的状态信息
    User user(userid, "", "", "offline");
    m_userModel.updateState(user);
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPassword(pwd);
    bool state = m_userModel.insert(user);
    if (state)
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    User user;

    {
        lock_guard<mutex> lock(m_connMutex);

        for (auto it = m_userConnMap.begin(); it != m_userConnMap.end(); ++it)
        {
            if (it->second == conn)
            {
                user.setId(it->first);
                m_userConnMap.erase(it);
                break;
            }
        }
    }

    // 用户注销，相当于就是下线，在redis中取消订阅通道
    m_redis.unsubscribe(user.getId());

    // 更新用户状态为offline
    if (user.getId() != -1)
    {
        user.setState("offline");
        m_userModel.updateState(user);
    }
}

// 一对一聊天
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int toid = js["to"].get<int>();

    {
        lock_guard<mutex> lock(m_connMutex);
        auto it = m_userConnMap.find(toid);
        if (it != m_userConnMap.end())
        {
            // 在线，推送消息
            it->second->send(js.dump());
            return;
        }
    }
    // 查询toid是否在其他服务器上在线
    User user = m_userModel.query(toid);
    if (user.getState() == "online")
    {
        m_redis.publish(toid, js.dump());
        std::cout << " ============== " << std::endl;
        std::cout << "publish " << std::endl;
        return;
    }

    // 离线，保存消息
    m_offlineMsgModel.insert(toid, js.dump());
}

// 添加好友业务
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    m_friendModel.insert(userid, friendid);
}

// 创建群组业务
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    string name = js["groupname"];
    string desc = js["groupdesc"];

    Group group(-1, name, desc); // id自增的，会在createGroup函数中再返回来
    if (m_groupModel.createGroup(group))
    {
        m_groupModel.addGroup(userid, group.getId(), "creator");
    }
}
// 加入群组业务
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    m_groupModel.addGroup(userid, groupid, "normal");
}
// 群组聊天业务
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = m_groupModel.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(m_connMutex);
    for (int id : useridVec)
    {
        auto it = m_userConnMap.find(id);
        if (it != m_userConnMap.end())
        {
            // 转发群消息
            it->second->send(js.dump());
        }
        else
        {
            // m_offlineMsgModel.insert(id, js.dump());
            // 查询toid是否在线
            User user = m_userModel.query(id);
            if (user.getState() == "online")
            {
                m_redis.publish(id, js.dump());
            }
            else
            {
                m_offlineMsgModel.insert(id, js.dump());
            }
        }
    }
}

void ChatService::redisHandler(int userid, string message)
{
    lock_guard<mutex> lock(m_connMutex);
    auto it = m_userConnMap.find(userid);
    if (it != m_userConnMap.end())
    {
        it->second->send(message);
        return;
    }

    // 如果发送者在发布消息到redis的时候接收者下线了，我们要存储该用户的离线消息
    m_offlineMsgModel.insert(userid, message);
}