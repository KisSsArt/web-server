#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <unistd.h> 
#include <cstring>
#include <thread>

#include "http.h"
#include "HttpData.h"

class WebServer {
public:
    WebServer(int port)
    {
        this->CreateSocket(port);
    }

    /* creating our socket here */
    void CreateSocket(int port)
    {
        std::cout << "Creating socket" << std::endl;

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        listen(serverSocket, 5); 
    }

    void CloseSocket()
    {
        std::cout << "Closing socket" << std::endl;

        close(serverSocket); 
    }

    int GetServerSocket() { return serverSocket; }

private:
    int serverSocket;
    sockaddr_in serverAddress;
};

char* CreateResponseHeader(char* response, HTTP_ver version, HTTP_STATUS status)
{
    std::string httpHeader(httpEmptyHeader);
    httpHeader.insert(5, version==HTTP_10 ? "1.0":"1.1");
    httpHeader.insert(9, statusStringMap[status]);

    std::strcpy(response, httpHeader.c_str());
    return response;
}

char* CreateResponsePacket(HTTPData* httpData)
{
    char* response = new char[1024];
    std::memset(response, '\0', 1024);

    if (!httpData->getValid())
        return CreateResponseHeader(response, httpData->getHTTPVersion(), HTTP_BAD_REQUEST);

    std::ifstream file(httpData->getFile());
    if (!file.is_open())
        return CreateResponseHeader(response, httpData->getHTTPVersion(), HTTP_NOT_FOUND);

    CreateResponseHeader(response, httpData->getHTTPVersion(), HTTP_OK);
    file.read(&(response[std::strlen(response)]), (1024 - std::strlen(response)));
    file.close();
    return response;
}

void ProcessRequest(int clientSocket)
{
    char* receiveBuffer = new char[1024];
    ssize_t packetLength = recv(clientSocket, receiveBuffer, 1024, 0);

    if (packetLength > 0)
    {
        HTTPData* httpData = new HTTPData(std::string(receiveBuffer));
        char* response = CreateResponsePacket(httpData);
        send(clientSocket, response, std::strlen(response), 0);
        delete[] response;
        delete httpData;
    }

    delete[] receiveBuffer;
    close(clientSocket);
}

int main()
{
    WebServer server(DEFAULT_SERVER_PORT);

    bool run = true;

    while (run)
    {
        int clientSocket = accept(server.GetServerSocket(), nullptr, nullptr); 
    
        std::thread clientProcess(ProcessRequest, clientSocket);

        clientProcess.join();
    }
  
    server.CloseSocket();

    return 0;
}