#include "../../include/server/chatservice.hpp"
#include "../../include/public.hpp"

// moduo库的日志
#include <muduo/base/Logging.h>

ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

ChatService::ChatService()
{
    m_msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    m_msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
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
    LOG_INFO << "doing login service!";
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    LOG_INFO << "doing reg service!";
}