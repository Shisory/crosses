#include "server.h"
#include "client.h"
#include "general.h"

int main()
{
    Server server;
    server.start("127.0.0.1", 1111);
    
    return 0;
}