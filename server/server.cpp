#include "server.h"
#include "client.h"
#include <iostream>
#include <cstring>
//#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

bool Server::initSocket(char* ip, int port)
{
    this->addr.sin_addr.s_addr = inet_addr(ip);
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);

#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
    {
        std::cerr << "error on WSA init" << std::endl;
        return false;
    }

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
#else
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif

    if (this->sock == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    if (bind(this->sock, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return false;
    }

    if (listen(this->sock, 10) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
        WSACleanup();
#else
        close(this->sock);
#endif
        return false;
    }

    return true;
}

void Server::acceptClients()
{
    std::cout << "Listening for connections: " << std::endl;
    while (true)
    {
        #ifdef _WIN32
        SOCKET socket = accept(this->sock, (SOCKADDR*)&this->addr, &this->sizeofaddr);
        #else
        socklen_t sizeofaddr = sizeof(this->addr);
        int clientSocket = accept(this->sock, (struct sockaddr*)&this->addr, &sizeofaddr);
        #endif
        {
            std::string ip = inet_ntoa(this->addr.sin_addr);
            Client client = Client(clientSocket, ip);
            clients.push_back(client);
            std::cout << "USER JOINED [" << &client << "]" << std::endl;

            std::thread t(&Server::handleClient, this, &clients.back());
            t.detach();
        }
    }
}

bool Server::start(char* ip, int port)
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
    {
        std::cerr << "error on WSA init" << std::endl;
        return false;
    }
#endif

    this->initSocket(ip, port);
    this->acceptClients();
    return true;
}

void Server::handleClient(Client* client)
{
    const char* msg = "HUY PIZDA";
    while (true)
    {
        std::cout << "Client [" << client << "] is in his own thread" << std::endl;
#ifdef _WIN32
        send(client->sock, msg, strlen(msg), 0);
#else
        send(client->sock, msg, strlen(msg), MSG_NOSIGNAL);
#endif
#ifdef _WIN32
        Sleep(2000);
#else
        usleep(2000000);
#endif
    }
}
