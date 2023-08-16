#pragma once
#include "general.h"
#include "client.h"

class Session
{
public:
// ---------------- DATA ------------------
    
    Client* first = nullptr;
    Client* second = nullptr;
    std::map<std::string, std::string> gameMap;
    std::map<std::string, int (Session::*)(char*)> commands;
    bool isFree;
    int id;
    int gameStatus = PENDING;

// ---------------- METHODS ---------------

    Session(int id, bool isFree);
    int fill(char* command);
    int exit(char* command);
    void matchmake(int status);
    void setJoinStatus();
    void assignClient(Client* client);

// ------------------ ENUM ------------------
    enum GameStatus
    {
        PENDING,
        STARTED,
        PROCESSING,
        CLIENT_1_MOVE,
        CLIENT_2_MOVE,
        OVER
    };

// ---------------- MAPS -------------------

    std::map<std::string, std::string> GAME_MAP
    {
        {"1", "*"},
        {"2", "*"},
        {"3", "*"},
        {"4", "*"},
        {"5", "*"},
        {"6", "*"},
        {"7", "*"},
        {"8", "*"},
        {"9", "*"},
        {"exit", "exit"},
        {"surrender", "restart"}
    };

};