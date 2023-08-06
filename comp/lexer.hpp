#pragma once

#include <string>
#include "token.hpp"
#include <fstream>
#include <unordered_map>

class Lexer
{
public:
    Token *scan();
    Lexer(std::fstream *s);
private:
    std::fstream *stream;
    int line;
    std::unordered_map<std::string, Token*> table;
};