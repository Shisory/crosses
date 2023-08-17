#include "general.h"
#include "client.h"
#include "session.h"

class Server
{
public:
// ---------------------- DATA --------------------

#ifdef _WIN32
    WSADATA wsaData;
    SOCKADDR_IN addr;
    SOCKET sock;
#else
    int sock;
    struct sockaddr_in addr;
#endif
    int sizeofaddr = sizeof(addr);
    std::mutex clientsMutex;
    std::mutex sessionsMutex;

    const char* moveCode = "move";
    const char* waitCode = "wait";

// ----------------------- VECTORS ---------------

    std::vector<std::thread> clientThreads;
    std::vector<Client> clients;
    std::vector<Session> sessions;

// ----------------------- METHODS ----------------

#ifdef _WIN32
    bool initWSA();
#endif

    bool initSocket(char* ip, int port);
    bool start(char* ip, int port);

    void acceptClients();
    void handleClient(Client* client);
    void joinUser(std::vector<Client>& clients, Session* sessions);
    void matchmakingThread();
    void startGame(Session* session);
    void handleGame(std::vector<Session>& sessions);
    

    Session* getOrCreateSession(std::vector<Session>& sessions);

};
