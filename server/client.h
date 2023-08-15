#pragma once
#include "general.h"

class Client
{
public:
// ----------------- DATA ------------------------

    int status = WAITING;
    char connectionBuffer[1024] = {0};
    char gameBuffer[100] = {0};
#ifdef _WIN32
    SOCKET sock;
#else
    int sock;
#endif

    std::string ip;
    

// --------------   METHODS -----------------------

    Client();
#ifdef _WIN32
    Client(SOCKET socket, std::string ip);
#else
    Client(int socket, std::string ip);
#endif
    bool setStatus(int status);
    bool isInGame();
    void clearBuffer();
    void clearGameBuffer();

// ----------------- ENUM -------------------------
    enum ClientStatus
    {
        BLANK,
        DISCONNECTED,
        WAITING,
        MATCHED,
        GAME
    };
};
    
