#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <thread>
#include <string>
#include <map>
#include <mutex>
#include <ctime>
//#include "commands.h"
#include <memory>

#ifdef _WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib") // Link the Winsock library
    #include <windows.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <pthread.h>
#endif


// enum GameStatus
// {
//     PENDING,
//     STARTED,
//     PROCESSING,
//     CLIENT_1_MOVE,
//     CLIENT_2_MOVE,
//     OVER
// };



// std::map<std::string, std::string> GAME_MAP
// {
//     {"1", "*"},
//     {"2", "*"},
//     {"3", "*"},
//     {"4", "*"},
//     {"5", "*"},
//     {"6", "*"},
//     {"7", "*"},
//     {"8", "*"},
//     {"9", "*"},
//     {"exit", "exit"},
//     {"surrender", "restart"}
// };

// //waiting
// std::map<std::string, std::string> WAITING_OR_OVER
// {
//     {"quit", "quit"},
//     {"revenge", "revenge"}
// };