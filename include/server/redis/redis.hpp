#ifndef REDIS_HPP
#define REDIS_HPP

#include <hiredis/hiredis.h>
#include <thread>
#include <functional>

using namespace std;

class Redis
{
public:

    Redis();
    ~Redis();

    // 连接
    bool connect();

    // 发布消息
    bool publish(int channel, string message);

    // 订阅
    bool subscribe(int channel);

    // 取消订阅
    bool unsubscribe(int channel);

    // 接受订阅通道中的消息
    void observer_channel_message();

    // 初始化回调
    void init_notify_handler(function<void(int,string)> fn);

private:
    redisContext* m_publish_context;
    redisContext* m_subscribe_context;

    function<void(int,string)> m_notify_message_handler;
};



#endif