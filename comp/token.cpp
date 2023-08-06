#include "token.hpp"
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