#include "server.h"
#include "client.h"
#include <iostream>
#include <cstring>
//#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

bool Server::initSocket(char* ip, int port)
{
    this->addr.sin_addr.s_addr = inet_addr(ip);
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);

#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
    {
        std::cerr << "error on WSA init" << std::endl;
        return false;
    }

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
#else
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif

    if (this->sock == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    if (bind(this->sock, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
#else
        close(this->sock);
#endif
        return false;
    }

    if (listen(this->sock, 10) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
#ifdef _WIN32
        closesocket(this->sock);
        WSACleanup();
#else
        close(this->sock);
#endif
        return false;
    }

    return true;
}

void Server::acceptClients()
{
    std::cout << "Listening for connections: " << std::endl;
    while (true)
    {
        #ifdef _WIN32
        SOCKADDR_IN clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET socket = accept(this->sock, (SOCKADDR*)&clientAddr, &clientAddrSize);
        #else
        socklen_t sizeofaddr = sizeof(this->addr);
        int socket = accept(this->sock, (struct sockaddr*)&this->addr, &sizeofaddr);
        #endif
        {
            std::string ip = inet_ntoa(this->addr.sin_addr);
            Client client = Client(socket, ip);
            std::lock_guard clientsLock(clientsMutex);
            clients.push_back(client);
            std::cout << "USER JOINED [" << &client << "]" << std::endl;
            std::cout << "Client address from vector: [" << &clients.back() << "]" << std::endl;

            clientThreads.emplace_back(&Server::handleClient, this, &clients.back());
        }
    }
}

bool Server::start(char* ip, int port)
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
    {
        std::cerr << "error on WSA init" << std::endl;
        return false;
    }
#endif

    this->initSocket(ip, port);
    this->clients.reserve(10);

    // --------------  STARTING THREADS FOR MANAGING CLIENTS AND ACTIVE GAMES -------------------------

    std::thread clientAcceptor(&Server::acceptClients, this);
    std::thread matchmaker(&Server::matchmakingThread, this);
    std::thread gameHandler(&Server::handleGame, this, std::ref(this->sessions));

    
    clientAcceptor.join();
    matchmaker.join();
    gameHandler.join();
    return true;
}

void Server::handleClient(Client* client)
{
    const char* msg = "msg from server";
    while (true)
    {
        //std::cout << "\n ---------------------------- \nClient [" << client << "] is in his own thread\n ---------------------------- \n" << std::endl;
#ifdef _WIN32
        //send(client->sock, msg, strlen(msg), 0);
#else
        send(client->sock, msg, strlen(msg), MSG_NOSIGNAL);
#endif
#ifdef _WIN32
        Sleep(2000);
#else
        usleep(2000000);
#endif
    }
}



Session* Server::getOrCreateSession(std::vector<Session>& sessions)
{
    for(auto& sess : sessions)
    {
        if(sess.isJoinable()){
            Sleep(1000);

            // std::cout << "\n************************ \nReturned already existing session. ID: " << sess.id << ". Status: " << sess.isJoinable() << std::endl;
            // std::cout << "\tsession first user: " << sess.first << 
            //             "\n\tsession second user: " << sess.second << "\n\n ************************ \n" << std::endl;

            return &sess;
        }
    }
    std::srand(std::time(0));
    sessions.emplace_back(Session(std::rand(), true));
    //std::cout << "Created new session. ID: " << sessions.back().id << std::endl;
    return &sessions.back();
}



void Server::matchmakingThread()
{
    while(true)
    {
        Sleep(1000);
        std::lock_guard<std::mutex> sessionLock(sessionsMutex);
        std::lock_guard<std::mutex> clientLock(clientsMutex);

        //std::cout << "inside matchmaker thread" << std::endl;
        Session* session = getOrCreateSession(this->sessions);

        for(Client& client : this->clients)
        {
            if(client.status == Client::WAITING)
            {
                session->assignClient(&client);
                session->recheckJoinStatus();
                if(!session->isJoinable()) this->startGame(session);

                break;
            }
                
        }
        
    }
}


void Server::startGame(Session* session)
{
    // setting client statuses to "In game"
    session->first->setStatus(Client::GAME);
    session->second->setStatus(Client::GAME);

    // setting first joined client to move first
    session->gameStatus = Session::CLIENT_1_MOVE;

}

void Server::handleGame(std::vector<Session>& sessions)
{
    while(true)
    {
        Sleep(2000);
        if(sessions.size())
        {
            std::lock_guard<std::mutex> sessionLock(sessionsMutex);
            std::lock_guard<std::mutex> clientLock(clientsMutex);

            for(Session& session : sessions)
            {
                if(!session.isJoinable())
                {
                    // std::cout << "\n ------------------- \nInside active session. Session id: " << session.id << " session status: " << session.gameStatus 
                    // << "\nAddress of first: " << session.first << "\nAddress of second: " << session.second 
                    // << "\n ------------------- \n" << std::endl;  

                    if(session.gameStatus == Session::CLIENT_1_MOVE)
                    {
                        // telling each client what to do
                        std::cout << "\nClient 1 turn to make move" << std::endl;
                        send(session.first->sock, this->moveCode, strlen(this->moveCode), 0);
                        send(session.second->sock, this->waitCode, strlen(this->waitCode), 0);

                        recv(session.first->sock, session.sessionBuffer, sizeof(session.sessionBuffer), 0);
                        std::cout << "\nClient 1 made move - " << session.sessionBuffer;
                        
                        
                        memset(session.sessionBuffer, 0, sizeof(session.sessionBuffer));
                        std::cout << "\nSetting client 2 turn to move";
                        session.setGameStatus(Session::CLIENT_2_MOVE);
                        break;
                            

                    } else if(session.gameStatus == Session::CLIENT_2_MOVE)
                    {
                        std::cout << "\nClient 2 turn to make move";
                        send(session.first->sock, this->waitCode, strlen(this->waitCode), 0);
                        send(session.second->sock, this->moveCode, strlen(this->moveCode), 0);
                        
                        recv(session.second->sock, session.sessionBuffer, sizeof(session.sessionBuffer), 0);
                        std::cout << "\nClient 2 made move - " << session.sessionBuffer;
                        std::cout << "\nSetting client 1 turn to move";
                        session.setGameStatus(Session::CLIENT_1_MOVE);
                        memset(session.sessionBuffer, 0, sizeof(session.sessionBuffer));
                        break;
                    }
                    else std::cout << "Weird session gamestatus encountered" << std::endl;
                }
            }
        }
    }   
}