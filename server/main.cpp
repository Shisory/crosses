#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <string>
#include <map>
#include <mutex>
#include <ctime>
#include "commands.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include <windows.h>

enum MoveState
{
    MOVE,
    WAIT
};

enum GameStatus
{
    PENDING,
    STARTED,
    PROCESSING,
    CLIENT_1_MOVE,
    CLIENT_2_MOVE,
    OVER
};

enum ClientStatus
{
    BLANK,
    DISCONNECTED,
    WAITING,
    MATCHED,
    GAME
};


class Connection
{
    public:
    //Session currentSession;
    Connection(){
    };
    Connection(SOCKET socket, std::string ip):sock{socket}, ip{ip}{};
    int status = ClientStatus::WAITING;
    char connectionBuffer[1024];
    char gameBuffer[100];
    SOCKET sock;
    std::string ip;
    
    bool setStatus(int status)
    {
        this->status = status;
        return true;
    };

    bool isInGame(){
        return this->status == ClientStatus::GAME;
    }

    void clearBuffer()
    {
        memset(this->connectionBuffer, 0, sizeof(this->connectionBuffer));
    }

    void clearGameBuffer()
    {
        memset(this->gameBuffer, 0, sizeof(this->gameBuffer));
    }
};

class Session{
    public:
    
    std::map<std::string, std::string> gameMap;
        
    Session(int id, bool isFree)
    {
        this->id = id;
        this->isFree = isFree;
        gameMap.insert(GAME_MAP.begin(), GAME_MAP.end());
    };
    bool isFree;
    int id;
    int gameStatus = GameStatus::PENDING;
    Connection* first;
    Connection* second;

    void matchmake(int status)
    {
        this->first->setStatus(status);
        this->second->setStatus(status);    
    };

    void setJoinStatus()
    {
        if(first->status == ClientStatus::BLANK || second->status == ClientStatus::BLANK)
        {
            this->isFree = true;
        } 
        else
        {
            this->isFree = false;
            this->gameStatus = GameStatus::STARTED; 
        }
    }

    // ~Session(){
    //     delete this->first;
    //     delete this->second;
    // };
};

void handleClient(Connection*);
void createGameSession(SOCKET sock1, SOCKET sock2);
void sessionJoiner(std::vector<Connection> &connections, std::vector<Session> &sessions);
Session* getOrCreateSession(std::vector<Session> &sessions);
void joinUser(std::vector<Connection> *connections, Session *session);
void gameHandler(std::vector<Session>& sessions);








std::vector<std::thread> clientThreads;
std::vector<Connection> connections;
std::vector<Session> sessions;

int  main()
{
     WSADATA wsaData;

    

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        WSACleanup();
        std::cout << "error on wsa init" << std::endl;
    };

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1111);

    SOCKET listenSocket = socket(addr.sin_family, SOCK_STREAM, 0);

    if(bind(listenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) // bind addresses to socket
        {
            std::cerr << "Error binding socket" << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        };

    listen(listenSocket, 10);

    std::cout << "listening for connections..." << std::endl;

    

    //int counter = 0;

    std::thread sessionCreator(sessionJoiner, std::ref(connections), std::ref(sessions));
    std::thread sessionHandler(gameHandler, std::ref(sessions));
    while(true)
    {
        //clientSockets.push_back(accept(listenSocket, (SOCKADDR*)&addr, &sizeofaddr));
        SOCKET socket = accept(listenSocket, (SOCKADDR*)&addr, &sizeofaddr);

        if(socket != INVALID_SOCKET)
        {
            std::string ip = inet_ntoa(addr.sin_addr);
            Connection connection = Connection(socket, ip);
            connections.push_back(connection);
            clientThreads.emplace_back(handleClient, &connections.back());
        }   

        
    }

    for (auto& thread : clientThreads) 
        {
            thread.join();
        }   

    closesocket(listenSocket);
    WSACleanup();
    return 0;


}

// placeholder for checking client connection
bool checkConnectionStatus(Connection *connection)
{
    return false;

}

void handleClient(Connection* connection){
    while(true)
    {   
        std::cout << "Client connected. Client IP: " << connection->ip << std::endl;
        

        char sendMsg[512] = "Waiting for another user (server message)";
        char recvBuff[1024];
        
        std::cout << "From client thread: in session, waiting for another player. Status: " << connection->status << std::endl;
        while(connection->status == ClientStatus::MATCHED || checkConnectionStatus(connection)) // TODO check connection status
        {
            std::cout << "From client thread: in while. Status - " << connection->status << std::endl;
            send(connection->sock, sendMsg, sizeof(sendMsg), 0);
            recv(connection->sock, recvBuff, sizeof(recvBuff), 0);
            std::cout << "message from client: " << recvBuff << std::endl;
            memset(recvBuff, 0, sizeof(recvBuff));
            Sleep(500);
        }
        // disconnected obrabotka

        while(connection->status == ClientStatus::GAME)
        {
            if(connection->gameBuffer != "/0")
            {
                if(connection->gameBuffer == "move")
                {
                    send(connection->sock, connection->gameBuffer, sizeof(connection->gameBuffer), 0);
                    connection->clearGameBuffer();
                    recv(connection->sock, connection->connectionBuffer, sizeof(connection->connectionBuffer), 0);

                }
                else if(connection->gameBuffer == "wait")
                {
                    send(connection->sock, connection->gameBuffer, sizeof(connection->gameBuffer), 0);
                    connection->clearGameBuffer();
                    
                }
            }
            Sleep(500);     
                
        };
        Sleep(1000);
    }
};

std::string validateMove(char* move, std::map<std::string, std::string>* commands)
{
    if(commands->find(move) != commands->end())
    {
        //if()
    }
}

Session* getOrCreateSession(std::vector<Session> &sessions)
{
    for(auto& sess : sessions)
    {
        if(sess.isFree){
            Sleep(1000);

            return &sess;
        }
            
    }
    std::srand(std::time(0));
    Session *sess = new Session(std::rand(), true);
    sess->first = new Connection();
    sess->first->status = ClientStatus::BLANK;
    sess->second = new Connection();
    sess->second->status = ClientStatus::BLANK;
    sessions.push_back(*sess);
    return &sessions.back();  // create session and return reference to it
}


void joinUser(std::vector<Connection> *connections, Session *session)
{
    if((*connections).size())
    {
        for(auto& usr : (*connections))
        {
            if(usr.status == ClientStatus::WAITING) //this status is by the default
            {
                session->first->status == ClientStatus::BLANK ? session->first = &usr :  session->second = &usr;
                usr.status = ClientStatus::MATCHED;
                break;
            }
        
        }
    }
    
}

void sessionStarter(Session* session)
{
    if(session->first->status == ClientStatus::MATCHED && session->second->status == ClientStatus::MATCHED)
    {
        session->matchmake(ClientStatus::GAME);
    }
}


void sessionJoiner(std::vector<Connection> &connections, std::vector<Session> &sessions)
{
    while(true)
    {
        Session* session = getOrCreateSession(sessions);
        joinUser(&connections, session);
        session->setJoinStatus(); 
        sessionStarter(session);
    }
}


int handleClientInput(Connection* first, Connection* second, int status) // possible pointer refactor
{
    if(status == GameStatus::CLIENT_1_MOVE)
    {

        strcpy(first->gameBuffer, "move");
        strcpy(second->gameBuffer, "wait");

        
        return GameStatus::CLIENT_2_MOVE;
        

        // send(2, zhdat);
        // send (1, hod);
        // recv(1, hod);
        // check, MAP -> zapisat'
        // status == client 2 move
    }
    else if (status == GameStatus::CLIENT_2_MOVE)
    {
        strcpy(second->gameBuffer, "move");
        strcpy(first->gameBuffer, "wait");
        return 2;   
    }

    else
    {
        std::cout << "something went wrong" << std::endl;
        return 3;
    }  
}

void gameHandler(std::vector<Session>& sessions)
{
    while(true){
        for(auto& sess : sessions){
            if(!sess.isFree){
                if(sess.gameStatus == GameStatus::OVER || sess.gameStatus == GameStatus::PENDING)
                {
                    // TODO somethig with invalid session
                }
                if(sess.first->isInGame() && sess.second->isInGame())
                {
                    
                   //sess. = handleClientInput(sess.first, sess.second, sess.gameStatus);

                }
            }

        }
    }
}