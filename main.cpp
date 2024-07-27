#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h> 
#include <thread>

#include "config.h"
#include "HttpData.h"
#include "server.h"

/*
    TODO: Parse argv
*/
int main(int argc, char* argv[])
{
    WebServer server(DEFAULT_SERVER_PORT);
    while (server.Good())
    {
        server.CreateProcessingThread(accept(server.GetServerSocket(), nullptr, nullptr));
    }

    server.CloseSocket();
    return 0;
}