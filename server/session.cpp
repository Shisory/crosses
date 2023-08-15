#include "session.h"

Session::Session(int id, bool isFree)
{
    this->id = id;
    this->isFree = isFree;
    gameMap.insert(GAME_MAP.begin(), GAME_MAP.end());
    this->commands["1"] = &Session::fill;
    this->commands["2"] = &Session::fill;
    this->commands["3"] = &Session::fill;
    this->commands["4"] = &Session::fill;
    this->commands["5"] = &Session::fill;
    this->commands["6"] = &Session::fill;
    this->commands["7"] = &Session::fill;
    this->commands["8"] = &Session::fill;
    this->commands["9"] = &Session::fill;
    this->commands["exit"] = &Session::exit;
    this->commands["surrender"] = &Session::exit;
    this->first = new Client();
    this->second = new Client();
};




int Session::fill(char* command)
{
    std::cout << "Client has moved. Field # : " << command << std::endl;
    
    return 0; 
}

int Session::exit(char* command)
{
    std::cout << "Client triggered exit" << std::endl;
}

void Session::matchmake(int status)
{
    this->first->setStatus(status);
    this->second->setStatus(status);    
};

void Session::setJoinStatus()
{
    if(this->first->status == ClientStatus::BLANK || this->second->status == ClientStatus::BLANK)
    {
        this->isFree = true;
    } 
    else
    {
        this->isFree = false;
        this->gameStatus = GameStatus::STARTED; 
    }
}

