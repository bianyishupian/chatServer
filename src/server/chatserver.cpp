#include "../../include/server/chatserver.hpp"
#include "../../include/server/chatservice.hpp"
#include "../../thirdparty/json.hpp"

#include <iostream>
#include <functional>
#include <string>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg)
    : m_loop(loop), m_server(loop, listenAddr, nameArg)
{
    // 注册连接回调函数
    m_server.setConnectionCallback(std::bind(&ChatServer::connectionCallback, this, _1));
    // 注册消息回调函数
    m_server.setMessageCallback(std::bind(&ChatServer::messageCallback, this, _1, _2, _3));
    // 设置线程池线程数量
    m_server.setThreadNum(4);
}

void ChatServer::start()
{
    m_server.start();
}

void ChatServer::connectionCallback(const TcpConnectionPtr &conn)
{
    // 客户端断开连接
    if (!conn->connected())
    {
        conn->shutdown(); // close(fd)
        // m_loop->quit();
    }
}
void ChatServer::messageCallback(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 反序列化
    json js = json::parse(buf);

    // 根据js[msgid]获取对应业务的handler
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn, js, time);
    
}