#include <iostream>
#include <arpa/inet.h> //#include <Winsock2.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // for windows -> #include <windows.h>
#include <thread>

class Session
{
public:
    Session(int connected_socket)
    {

    }
};


class GameTCPClient
{   
public:
    int client_socket;
    int connection_status;
    int read_status;
    char buffer[1024] = {0};
    struct sockaddr_in server_address;

    char *ip;
    int port;
    

    GameTCPClient(char* ip, int port)
    {
        this->ip = ip;
        this->port = port;
    }

    void init_socket()
    { 
        this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (this->client_socket == -1)
        {
            std::cout << "Failed to create client socket" << std::endl;
            exit(EXIT_FAILURE);
        }
        std:: cout << "Client socket created" << std::endl;
    }

    void init_server_address()
    {
        this->server_address.sin_family = AF_INET;
        this->server_address.sin_port = htons(this->port); // host to network short(big endian)
        // mb error because of ip is declared below
        int status = inet_pton(AF_INET, this->ip, &this->server_address.sin_addr); // convert IPv4 and IPv6 addresses from text to binary form
        if (status == -1)
        {
            std::cout << "Invalid address" << std::endl;
            exit(EXIT_FAILURE);
        }
        else if (status == 0)
        {
            std::cout << "Address not supported" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "Address supported" << std::endl;
        }
    }

    Session connect_to_server()
    {
        this->init_socket();
        this->init_server_address();
        short attempts = 0;
        while(attempts < 5)
        {
            attempts++;

            this->connection_status = connect(
                this->client_socket,
                (struct sockaddr *)&this->server_address,
                sizeof(this->server_address)
            );
            if (this->connection_status == -1)
            {
                std::cout << "Connection failed |" << " Attempts left ->" << attempts << std::endl;
                sleep(1);
                continue;
                exit(EXIT_FAILURE);
            }
            else
            {
                std::cout << "Connection established" << std::endl;
            }
            return Session(this->client_socket);
        }
    }
};

int main()
{
    GameTCPClient client("127.0.0.1", 8080);
    Session session = client.connect_to_server();
}