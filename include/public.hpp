#ifndef PUBLIC_HPP
#define PUBLIC_HPP

enum EnMsgType
{
    LOGIN_MSG = 1,      // 登录消息
    LOGIN_MSG_ACK,      // 登录消息确认
    LOGINOUT_MSG,       // 注销消息
    REG_MSG,            // 注册消息
    REG_MSG_ACK,        // 注册消息确认
    ONE_CHAT_MSG,       // 聊天信息
    ADD_FRIEND_MSG,     // 添加好友消息
    CREATE_GROUP_MSG,   // 创建群组消息
    ADD_GROUP_MSG,      // 加入群组消息
    GROUP_CHAT_MSG,     // 群组聊天消息
};

#endif