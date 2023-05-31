#ifndef CHATSERVICE_HPP
#define CHATSERVICE_HPP

#include "../../thirdparty/json.hpp"
#include "model/offlinemsgmodel.hpp"
#include "model/usermodel.hpp"
#include "model/friendmodel.hpp"
#include "model/groupmodel.hpp"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
using json = nlohmann::json;

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

class ChatService
{
public:
    // 获取单例对象
    static ChatService* instance();
    // 处理登录
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理退出
    void loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 根据msgid获取对应的回调函数
    MsgHandler getHandler(int msgid);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 服务端退出的业务重置
    void reset();

private:
    ChatService();
    // 存储回调的映射
    std::unordered_map<int, MsgHandler> m_msgHandlerMap;
    // 存储连接的映射
    std::unordered_map<int, TcpConnectionPtr> m_userConnMap;
    // 保证m_userConnMap的线程安全
    mutex m_connMutex;
    UserModel m_userModel;
    FriendModel m_friendModel;
    GroupModel m_groupModel;
    OfflineMsgModel m_offlineMsgModel;
};

#endif