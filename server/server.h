#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library

class Server 
{
public:
    

    SOCKET listenSocket;
    SOCKADDR_IN addr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    Server(const int port, char *ip);
    ~Server();

    SOCKET create_socket();

    void handleClient();

    class Client
    {
        public:

        private:
    };


private:
    WSADATA wsaData;
    std::vector<std::thread> clientThreads;
};