#include "session.h"

Session::Session(int id, bool isFree)
{
    this->id = id;
    //this->isFree = isFree;
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



void Session::assignClient(Client* client)
{
    std::cout << "Session id: " << this->id << std::endl;
    if(!this->first)
    {
        this->first = client;
        client->status = Client::MATCHED;
        std::cout << "Assigning client into first session slot" << std::endl;
    }
    else if(!this->second)
    {
        this->second = client;
        client->status = Client::MATCHED;
        std::cout << "Assigning client into second session slot" << std::endl;
    }
    else std::cout << "Trying to assign client to a full session" << std::endl;
}

bool Session::isJoinable(){
    return this->isFree;
}

void Session::recheckJoinStatus()
{
    if(this->first && this->second)
    {
        this->isFree = false;
        this->gameStatus = STARTED;
        std::cout << "Session is full. Starting a game....." << std::endl;
    }
}

void Session::setGameStatus(int status)
{
    this->gameStatus = status;
}

// Player 1 always puts 'X', player 2 always puts 'O' in a map cell

bool Session::validateMove(char* move, const char playerCode) // TODO rename to validateAndSetMove
{
    if (this->gameMap.find(move) != this->gameMap.end()) 
    {
        
        if(strcmp(gameMap[move].c_str(), "*") == 0)
        {
            this->gameMap[move] = playerCode;
            std::cout << "\nPut player code " << playerCode << "in cell " << move << std::endl;
            return true;
        }
//        else if(strcmp(gameMap[move].c_str(), "X") == 0 || strcmp(gameMap[move].c_str(), "O") == 0)
//        {
//            std::cout << "This cell is already taken" << std::endl;
//            return false;
//        }
        else
        {
            std::cout << "Field is taken" << std::endl; //TODO: SEND MESSAGE TO USER
        }
    } 
    else 
    {
        std::cout << "this key is not present in map\n";
        return false;
    }
}


bool Session::checkWin(){
    if( 
        //  checking rows to be win combination
        ((this->gameMap["1"] == this->gameMap["2"] && this->gameMap["3"] == this->gameMap["2"]) && (this->gameMap["1"] != "*")) ||
        ((this->gameMap["4"] == this->gameMap["5"] && this->gameMap["6"] == this->gameMap["5"]) && (this->gameMap["4"] != "*")) ||
        ((this->gameMap["7"] == this->gameMap["8"] && this->gameMap["9"] == this->gameMap["8"]) && (this->gameMap["7"] != "*")) ||
        // checking columns to be win combination
        ((this->gameMap["1"] == this->gameMap["4"] && this->gameMap["7"] == this->gameMap["4"]) && (this->gameMap["1"] != "*")) ||
        ((this->gameMap["2"] == this->gameMap["5"] && this->gameMap["8"] == this->gameMap["5"]) && (this->gameMap["2"] != "*")) ||
        ((this->gameMap["3"] == this->gameMap["6"] && this->gameMap["9"] == this->gameMap["6"]) && (this->gameMap["3"] != "*")) ||
        // checking diagonals
        ((this->gameMap["1"] == this->gameMap["5"] && this->gameMap["9"] == this->gameMap["5"]) && (this->gameMap["1"] != "*")) || 
        ((this->gameMap["3"] == this->gameMap["5"] && this->gameMap["7"] == this->gameMap["5"]) && (this->gameMap["3"] != "*")) )
    {
        
        this->setGameStatus(Session::OVER);
        return true;
    } 
    else return false;


}