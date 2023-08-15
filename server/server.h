#include "general.h"
#include "client.h"
//#include "session.h"

class Server
{
public:
// ---------------------- DATA --------------------

    WSADATA wsaData;
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    SOCKET sock;
    std::mutex clientsMutex;
    std::mutex sessionsMutex;

// ----------------------- VECTORS ---------------

    std::vector<std::thread> clientThreads;
    std::vector<Client> clients; 
    // std::vector<Session> sessions;   

// ----------------------- METHODS ----------------

    bool initWSA(); 
    bool initSocket(char* ip, int port);
    void acceptClients();
    bool start(char* ip, int port);
    void handleClient(Client* client);
};