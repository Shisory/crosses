#include "client.h"
#include <unistd.h>


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

void Client::receiveClientData()
{
    int bytesReceived = recv(this->sock, this->clientBuffer + this->totalBytesRecived, sizeof(this->clientBuffer) - this->totalBytesRecived, 0);
    if(bytesReceived == -1)
    {
        std::cout << "ERROR HAPPEND WITH RECEIVING BYTES FOR CLIENT -> " << this << std::endl;
    }
    else if(bytesReceived == 0)
    {
        // do nothing
    }
    else
    {
        totalBytesRecived += bytesReceived;
    }
    if (this->totalBytesRecived == MESSAGE_SIZE)
    {
        this->totalBytesRecived = 0;
        this->isClientBufferFull = true;
    }
    usleep(1000000 * 2);
}