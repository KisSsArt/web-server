#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class WebServer {
public:
    WebServer(int port) { this->CreateSocket(port); }

    void CreateSocket(int port);
    void CloseSocket();

    void CreateProcessingThread(int clientSocket);

    bool Good() { return this->good; }
    int GetServerSocket() { return this->serverSocket; }

private:
    bool good;
    int serverSocket;
    sockaddr_in serverAddress;
};