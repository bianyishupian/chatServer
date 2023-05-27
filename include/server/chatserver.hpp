#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg);
    void start();

private:
    // 处理用户的连接与断开
    void connectionCallback(const TcpConnectionPtr &conn);
    // 处理用户的读写事件
    void messageCallback(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time);

    TcpServer m_server; // moduo库实现服务器功能的类对象
    EventLoop *m_loop;  // 指向事件循环对象的指针
};


#endif