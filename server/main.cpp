#include "server.h"
#include "client.h"

int  main()
{
    try
    {
        Server *server = new Server(1111, "127.0.0.1"); // initialize server instance
        
        server->listenSocket = server->create_socket(); //create listen socket

        if (server->listenSocket == INVALID_SOCKET)
        {
            std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        if(bind(server->listenSocket, (SOCKADDR*)&server->addr, sizeof(server->addr)) == SOCKET_ERROR) // bind addresses to socket
        {
            std::cerr << "Error binding socket" << std::endl;
            closesocket(server->listenSocket);
            WSACleanup();
            return 1;
        };

        listen(server->listenSocket, 10); //listen up to 10 connections




        while(true)
        {
            SOCKET clientConnection = accept(server->listenSocket, (SOCKADDR*)&server->clientAddr, &server->clientAddrLen);
            
        }
        delete server;
    }

    catch(const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }



    
    WSACleanup();
    
    return 0;
}