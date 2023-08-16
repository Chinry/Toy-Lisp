#pragma once

#include <string>
#include "token.hpp"
#include <fstream>
#include <unordered_map>

class Lexer
{
public:
    Token *scan();
    Lexer(std::iostream *s);
private:
    std::iostream *stream;
    int line;
    std::unordered_map<std::string, Token*> table;
    int uniqueNum;
};