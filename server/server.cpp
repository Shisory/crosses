#include "server.h"

bool Server::initWSA()
{
    if(WSAStartup(MAKEWORD(2,2), &this->wsaData) != 0)
    {
        WSACleanup();
        //std::cout << "error on wsa init" << std::endl;
        return false;
    }
    return true;
}

bool Server::initSocket(char* ip, int port)
{
    this->addr.sin_addr.s_addr = inet_addr(ip);
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->sock = socket(addr.sin_family, SOCK_STREAM, 0);

    if(bind(this->sock, (SOCKADDR*)&this->addr, sizeof(this->addr)) == SOCKET_ERROR) // bind addresses to socket
    {
        //std::cerr << "Error binding socket" << std::endl;
        closesocket(this->sock);
        WSACleanup();
        return false;
    }
    else
    {
        listen(this->sock, 10); // this-> error??
        return true;
    }
}

void Server::handleClients()
{
    while(true)
    {
        //std::cout << "READY TO ACCEPT CONNECTIONS" << std::endl;
        SOCKET socket = accept(this->sock, (SOCKADDR*)&this->addr, &this->sizeofaddr);

        if(socket != INVALID_SOCKET)
        {
            std::string ip = inet_ntoa(addr.sin_addr);
            // // Connection connection = Connection(socket, ip);
            // connections.push_back(connection);
            // std::cout << "USER JOINED [" << &connections.back() << "]" << std::endl;
            // clientThreads.emplace_back(handleClient, &connections.back());
        }   
    }
}

bool Server::start(char* ip, int port)
{
    this->initWSA();
    this->initSocket(ip, port);
    

    this->handleClients();
} 
