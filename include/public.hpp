#ifndef PUBLIC_HPP
#define PUBLIC_HPP

enum EnMsgType
{
    LOGIN_MSG = 1,      // 登录消息
    LOGIN_MSG_ACK,      // 登录消息确认
    REG_MSG,            // 注册消息
    REG_MSG_ACK,        // 注册消息确认
    ONE_CHAT_MSG,       // 聊天信息
    ADD_FRIEND_MSG,     // 添加好友消息
};

#endif