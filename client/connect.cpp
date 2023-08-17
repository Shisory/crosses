#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include <windows.h>

int main(){

    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        WSACleanup();
        std::cout << "error on wsa init" << std::endl;
    };

    SOCKADDR_IN addr;
     addr.sin_addr.s_addr = inet_addr("127.2.0.1");
     addr.sin_family = AF_INET;
     addr.sin_port = htons(1111);

    SOCKET connectionSocket = socket(addr.sin_family, SOCK_STREAM, 0);

    if(connect(connectionSocket, (SOCKADDR*)&addr, sizeof(addr)) != 0){
        std::cout << "error connecting to server";
        return 1;
    };

    std::cout << "Connected to server. Waiting for data to receive" << std::endl;
    char rcvBuff[512] {0};
    char sendBuff[512] {0};


    while(true){
       
        recv(connectionSocket, rcvBuff, sizeof(rcvBuff), 0);
        if(strcmp(rcvBuff, "move") == 0)
        {
            std::cout << "\nIt's your move:";
            std::cin >> sendBuff;
            
            send(connectionSocket, sendBuff, sizeof(sendBuff), 0);
            memset(rcvBuff, 0, sizeof(sendBuff));
            memset(sendBuff, 0, sizeof(rcvBuff));
        }
        else
        if(strcmp(rcvBuff, "wait") == 0)
        {
            std::cout << "\nWaiting for opponent's move";
            
            memset(rcvBuff, 0, sizeof(sendBuff));

        }
        else std::cout << "processing..." << std::endl;
        memset(rcvBuff, 0, sizeof(sendBuff));
        memset(sendBuff, 0, sizeof(rcvBuff));
    }


    return 0;
}