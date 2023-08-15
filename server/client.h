#pragma once
#include "general.h"

class Client
{
public:
// ----------------- DATA ------------------------

    int status = WAITING;
    char connectionBuffer[1024] = {0};
    char gameBuffer[100] = {0};
    SOCKET sock;
    std::string ip;
    

// --------------   METHODS -----------------------

    Client();
    Client(SOCKET socket, std::string ip);
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
    
