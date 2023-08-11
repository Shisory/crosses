#pragma once

#include <string>
#include <map>

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
    {"surrender", "ya_ebal"}
};

//waiting
std::map<std::string, std::string> WAITING_OR_OVER
{
    {"quit", "quit"},
    {"revenge", "revenge"}
};