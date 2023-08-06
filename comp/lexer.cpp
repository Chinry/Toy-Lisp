#include "lexer.hpp"
#include "token.hpp"
#include <ctype.h>
#include <sstream>
#include <fstream>

Lexer::Lexer(std::fstream *s)
{
    stream = s;
    std::unordered_map<std::string, Token*> m;
    table = m;
    line = 0;

    table["lambda"] = new Token(LAMBDA);
    table["let"] = new Token(LET);
    table["define"] = new Token(DEFINE);
    table["cond"] = new Token(COND);
    table["#t"] = new Token(TRUE);
    table["#f"] = new Token(FALSE);
    table["list"] = new Token(LIST);
    table["cons"] = new Token(CONS);
    table["cdr"] = new Token(CDR);
    table["car"] = new Token(CAR);
    table["eq?"] = new Token(EQUALS);
    table["null?"] = new Token(ISNULL);
    table["quote"] = new Token(QUOTE);
    table["modulo"] = new Token(MODULO);
    
}

Token Lexer::scan()
{
    char peek = ' ';
    for( ; ; stream->get(peek))
    {
        if(stream->eof())
        {
            Token t(END);
            return t;
        }
        switch(peek)
        {
        case ' ':
        case '\t':
            continue;
        case '\n':
            line++;
            continue;
        case '(':
            {
                Token t(OPENPAREN); 
                return t;
            }   
        case ')':
            {
                Token t(CLOSEPAREN); 
                return t;
            }
        case '+':
            {
                Token t(ADDITION); 
                return t;
            }
        case '-':
            {
                Token t(SUBTRACTION); 
                return t;
            }
        case '/':
            {
                Token t(DIVISION); 
                return t;
            }
        case '*':
            {
                Token t(MULTIPLICATION); 
                return t;
            }
        case '\'':
            {
                Token t(QUOTE);
                return t;
            }
        default:
            break;
        }
    }
    if(isdigit(peek))
    {
        int v = 0;
        do 
        {
            v = 10 * v + (peek - '0');
            stream->get(peek);
        } while(isdigit(peek));
        return Num(v);
    }
    if(isalpha(peek) || peek == '#')
    {
        std::stringbuf b;
        do
        {
            b.sputc(peek);
            stream->get(peek);
        }while (isalnum(peek));
        auto t = table.find(b.str());
        if (t != table.end()) return *(t->second);
        Word w(b.str());
        return w;
    }
    Token t(ID);
    return t;

}