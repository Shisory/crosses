#include "server.h"

int main()
{
    Server* server = new Server();
    server->start("127.0.0.1", 1111);
    std::cout << "fsaf" << std::endl;
    
    return 0;
}