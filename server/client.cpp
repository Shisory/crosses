#include "client.h"

Client::Client()
{
    this->status = BLANK;
}

#ifdef _WIN32
Client::Client(SOCKET socket, std::string ip)
#else
Client::Client(int socket, std::string ip)
#endif
{
    this->sock = socket; 
    this->ip = ip;
}

bool Client::setStatus(int status)
{
    this->status = status;
    return true;
};

bool Client::isInGame()
{
    return this->status == GAME;
}

void Client::clearClientBuffer() {
    memset(this->clientBuffer, 0, sizeof(this->clientBuffer));
}

void Client::clearGameBuffer()
{
    memset(this->gameBuffer, 0, sizeof(this->gameBuffer));
}