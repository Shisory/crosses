#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include <windows.h>

class MockClient
{
public:
    WSADATA wsaData;
    SOCKADDR_IN addr;
    SOCKET connectionSocket;

    bool initClient();
    bool connect();
};