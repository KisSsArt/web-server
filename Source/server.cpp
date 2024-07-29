#include "server.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <unistd.h> 
#include <cstring>
#include <thread>

#include "HttpData.h"

void WebServer::CreateSocket(int port)
{
    std::cout << "Creating socket" << std::endl;

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(port);
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->GetServerSocket(), (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));

    listen(this->GetServerSocket(), 5);

    this->good = true;
}

void WebServer::CloseSocket()
{
    std::cout << "Closing socket" << std::endl;

    close(this->GetServerSocket());

    this->good = false;
}

void RequestHandler(int clientSocket)
{
    char* receiveBuffer = new char[1024];
    ssize_t packetLength = recv(clientSocket, receiveBuffer, 1024, 0);

    if (packetLength > 0)
    {
        http::HTTPData* httpData = new http::HTTPData(std::string(receiveBuffer));
        char* response = httpData->CreateResponsePacket();
        send(clientSocket, response, std::strlen(response), 0);
        delete[] response;
        delete httpData;
    }

    delete[] receiveBuffer;
    close(clientSocket);
}

void WebServer::CreateProcessingThread(int clientSocket)
{
    std::thread clientHandle(RequestHandler, clientSocket);

    clientHandle.detach();
}