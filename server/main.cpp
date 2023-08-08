#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <string>
#include <unordered_map>
#include <mutex>
#include <ctime>


#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include <windows.h>

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
    SOCKET sock;
    std::string ip;
    bool setStatus(int status)
    {
        this->status = status;
        return true;
    };
};

class Session{
    public:
    Session(int id, bool isFree)
    {
        this->id = id;
        this->isFree = isFree;
    };
    bool isFree;
    int id;
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
            memset(recvBuff, 0, sizeof(recvBuff));
            std::cout << "INGAME" << std::endl;
            char sendMsgGame[512] = "GAME STARTED (server message)";
            send(connection->sock, sendMsgGame, sizeof(sendMsgGame), 0);
            recv(connection->sock, recvBuff, sizeof(recvBuff), 0);
            std::cout << "message from client IN GAME: " << recvBuff << std::endl;
            Sleep(1000);
                
        }
        Sleep(1000);
    }
};


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




