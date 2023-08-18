#pragma once
#include "general.h"

class Client
{
public:
// ----------------- DATA ------------------------

    int status = WAITING;

    char clientBuffer[100] = {0};
    bool isClientBufferFull = false;
    char gameBuffer[100] = {0};
    bool isGameBufferFull = false;
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
    void clearClientBuffer();
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
    
