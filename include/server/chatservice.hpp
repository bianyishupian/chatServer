#ifndef CHATSERVICE_HPP
#define CHATSERVICE_HPP

#include "../../thirdparty/json.hpp"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>

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
    // 处理注册
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 根据msgid获取对应的回调函数
    MsgHandler getHandler(int msgid);

private:
    ChatService();

    std::unordered_map<int, MsgHandler> m_msgHandlerMap;

};

#endif