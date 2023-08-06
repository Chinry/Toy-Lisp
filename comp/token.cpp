#include "token.hpp"
#include <iostream>
Token::Token(enum TokenId t)
{
    tag = t;
}

Num::Num(int val) : Token(NUM)
{
    value = val;
}

Word::Word(std::string lex) : Token(ID)
{
    lexeme = lex;
}

std::string Token::toString()
{
    switch (tag)
    {
        case OPENPAREN:
            return "(";
        case CLOSEPAREN:
            return ")";
        case NUM:
            return "Number";
        case ID:
            return "Id";
        case ADDITION:
            return "+";
        case DEFINE:
            return "define";
        default:
            return "not handled " + std::to_string(tag);  

    }
}