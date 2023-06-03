#include "../../../include/server/redis/redis.hpp"

#include <iostream>

using namespace std;

Redis::Redis()
    : m_publish_context(nullptr), m_subscribe_context(nullptr)
{
}

Redis::~Redis()
{
    if (m_publish_context != nullptr)
    {
        redisFree(m_publish_context);
    }
    if (m_subscribe_context != nullptr)
    {
        redisFree(m_subscribe_context);
    }
}

// 连接
bool Redis::connect()
{
    m_publish_context = redisConnect("127.0.0.1", 6379);
    if (m_publish_context == nullptr)
    {
        cerr << "redisConnect failed" << endl;
        return false;
    }
    m_subscribe_context = redisConnect("127.0.0.1", 6379);
    if (m_subscribe_context == nullptr)
    {
        cerr << "redisConnect failed" << endl;
        return false;
    }

    thread t([&](){
        observer_channel_message();
    });

    t.detach();
    cout << "connect redis-server success" << endl;

    return true;
}

// 发布消息
bool Redis::publish(int channel, string message)
{
    redisReply *reply = (redisReply *)redisCommand(this->m_publish_context, "PUBLISH %d %s", channel, message.c_str());
    if (reply == nullptr)
    {
        cerr << "publish command failed" << endl;
        return false;
    }
    std::cout << "redis publish success" << std::endl;
    freeReplyObject(reply);
    return true;
}

// 订阅
// 为什么订阅和取消订阅不和发布一样用redisCommand呢？因为redisCommand就是redisAppendCommand+redisBufferWrite+redisGetReply
// 而redisGetReply是阻塞的。当没有回应时会一直阻塞在这
// 发布消息的redis命令会立即得到响应，不会阻塞掉，但是订阅不会马上得到响应，如果使用redisCommand会阻塞到这里
// 所以我们分开来使用，另外开一个线程去调用redisGetReply
bool Redis::subscribe(int channel)
{
    std::cout << " ========== " << std::endl;
    std::cout << " subscribe " << std::endl;
    if (redisAppendCommand(this->m_publish_context, "SUBSCRIBE %d", channel) == REDIS_ERR)
    {
        cerr << "subscribe command failed!" << endl;
        return false;
    }
    int done = 0;
    while (!done)
    {
        std::cout << " ========== " << std::endl;
        std::cout << " subscribe " << std::endl;
        if (redisBufferWrite(this->m_subscribe_context, &done) == REDIS_ERR)
        {
            cerr << "redisBufferWrite failed!" << endl;
            return false;
        }
    }
    std::cout << " ========== " << std::endl;
    std::cout << " subscribe " << std::endl;
    return true;
}

// 取消订阅
bool Redis::unsubscribe(int channel)
{
    if (redisAppendCommand(this->m_publish_context, "UNSUBSCRIBE %d", channel) == REDIS_ERR)
    {
        cerr << "unsubscribe command failed!" << endl;
        return false;
    }
    int done = 0;
    while (!done)
    {
        if (redisBufferWrite(this->m_subscribe_context, &done) == REDIS_ERR)
        {
            cerr << "redisBufferWrite failed!" << endl;
            return false;
        }
    }
    return true;
}

// 接受订阅通道中的消息，在独立的一个线程中
void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while (REDIS_OK == redisGetReply(this->m_subscribe_context, (void **)&reply))
    {
        std::cout << " observer " << std::endl;
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            m_notify_message_handler(atoi(reply->element[1]->str), reply->element[2]->str);
            std::cout << " observer " << std::endl;
        }
        freeReplyObject(reply);
    }

    cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<" << endl;
}

// 初始化回调
void Redis::init_notify_handler(function<void(int, string)> fn)
{
    this->m_notify_message_handler = fn;
}