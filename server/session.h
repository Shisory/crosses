#pragma once
#include "general.h"
#include "client.h"

class Session
{
public:
// ---------------- DATA ------------------
    
    Client* first;
    Client* second;
    std::map<std::string, std::string> gameMap;
    std::map<std::string, int (Session::*)(char*)> commands;
    bool isFree;
    int id;
    int gameStatus = GameStatus::PENDING;


// ---------------- METHODS ---------------

    Session(int id, bool isFree);
    int fill(char* command);
    int exit(char* command);
    void matchmake(int status);
    void setJoinStatus();
};