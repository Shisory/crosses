#include "server.h"

bool Server::initWSA()
{
    if(WSAStartup(MAKEWORD(2,2), &this->wsaData) != 0)
    {
        WSACleanup();
        std::cerr << "error on wsa init" << std::endl;
        return false;
    }
    return true;
}

bool Server::initSocket(char* ip, int port)
{
    this->addr.sin_addr.s_addr = inet_addr(ip);
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->sock = socket(addr.sin_family, SOCK_STREAM, 0);

    if(bind(this->sock, (SOCKADDR*)&this->addr, sizeof(this->addr)) == SOCKET_ERROR) // bind addresses to socket
    {
        std::cerr << "Error binding socket" << std::endl;
        closesocket(this->sock);
        WSACleanup();
        return false;
    }
    else
    {
        listen(this->sock, 10); // this-> error??
        return true;
    }
}

void Server::acceptClients()
{
    std::cout << "Listening for connections: " << std::endl;
    while(true)
    {
        SOCKET socket = accept(this->sock, (SOCKADDR*)&this->addr, &this->sizeofaddr);

        if(socket != INVALID_SOCKET)
        {
            std::string ip = inet_ntoa(addr.sin_addr);
            Client client = Client(socket, ip);
            clients.push_back(client);
            std::cout << "USER JOINED [" << &client << "]" << std::endl;
            std::cout << "CLIENTS->BACK [" << &clients.back() << "]" << std::endl;
            std::cout << "CLIENTS->BACK 222[" << &clients.back() << "]" << std::endl;

            // clientThreads.emplace_back([this, &client] {
            //     handleClient(&client); //TODO: CHECK HOW IT WORKS (zalupka)
            // });
            // clientThreads.back().join();
            std::thread t(&Server::handleClient, this, &this->clients.back());
            t.join();
        }   
    }
}

bool Server::start(char* ip, int port)
{
    this->initWSA();
    this->initSocket(ip, port);
    this->acceptClients();
    return true;
} 

void Server::handleClient(Client* client)
{   
    char msg[25] = "HUY PIZDA";
    while(true)
    {
        std::cout << "Client [" << client << "] is in his own thread" << std::endl;
        // memset(msg, 0, sizeof(msg));
        send(client->sock, msg, sizeof(msg), 0);
        Sleep(2000);
    }
}
