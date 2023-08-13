
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <string>
#include <map>
#include <mutex>
#include <ctime>
//#include "commands.h"
#include <memory>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include <windows.h>

class Server
{
public:
    WSADATA wsaData;
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    SOCKET sock;


    bool initWSA(); 
    bool initSocket(char* ip, int port);
    void handleClients();
    bool start(char* ip, int port);
};