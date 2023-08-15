#include "client.h"

Client::Client()
{
    this->status = BLANK;
}

Client::Client(SOCKET socket, std::string ip)
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

void Client::clearBuffer()
{
    memset(this->connectionBuffer, 0, sizeof(this->connectionBuffer));
}

void Client::clearGameBuffer()
{
    memset(this->gameBuffer, 0, sizeof(this->gameBuffer));
}