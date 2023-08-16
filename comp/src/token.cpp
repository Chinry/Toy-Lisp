#include "token.hpp"
#include <iostream>
Token::Token(enum TokenId t)
{
    tag = t;
}

Token::Token(void)
{}

Num::Num(int val) : Token(NUM)
{
    value = val;
}

Num::Num() : Token(NUM){}

Word::Word(std::string lex, int id) : Token(ID)
{
    lexeme = lex;
    identifier = id;
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