#include "../../include/server/chatserver.hpp"
#include "../../include/server/chatservice.hpp"

#include <iostream>
#include <signal.h>

using namespace std;

void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main()
{
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr("127.0.0.1",10000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();
    loop.loop();

    
    return 0;
}