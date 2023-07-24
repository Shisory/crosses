#include "server.h"



Server::Server(const int port, char* ip)
{
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) 
        throw std::runtime_error("Failed to init socket lib");

     addr.sin_addr.s_addr = inet_addr(ip);
     addr.sin_family = AF_INET;
     addr.sin_port = htons(port);

     
};

Server::~Server()
{
    WSACleanup();
}

SOCKET Server::create_socket()
{
    return socket(addr.sin_family, SOCK_STREAM, 0);
}

void Server::handleClient()
{
    
}