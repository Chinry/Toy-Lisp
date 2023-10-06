#include "lexer.hpp"
#include "token.hpp"
#include <ctype.h>
#include <sstream>
#include <fstream>

Lexer::Lexer(std::iostream *s)
{
    uniqueNum = 10;
    stream = s;
    std::unordered_map<std::string, Token*> m;
    table = m;
    line = 0;
    end = new Token(END);

    table["lambda"] = new Token(LAMBDA);
    table["if"] = new Token(IF);
    table["let"] = new Token(LET);
    table["define"] = new Token(DEFINE);
    table["cond"] = new Token(COND);
    table["#t"] = new Token(TRUE);
    table["#f"] = new Token(FALSE);
    table["list"] = new Token(LIST);
    table["cons"] = new Token(CONS);
    table["cdr"] = new Token(CDR);
    table["car"] = new Token(CAR);
    table["null?"] = new Token(ISNULL);
    table["quote"] = new Token(QUOTE);
    table["modulo"] = new Token(MODULO);
    
}

Lexer::~Lexer()
{
    for(auto kv : table)
    {
        delete kv.second;
    }
    delete end;
}

Token *Lexer::scan()
{
    char peek = ' ';
    for( ; ; stream->get(peek))
    {
        if(stream->eof())
        {
            return end;
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
                return new Token(OPENPAREN);   
        case ')':
                return new Token(CLOSEPAREN);
        case '+':
                return new Token(ADDITION);
        case '-':
                return new Token(SUBTRACTION);
        case '/':
                return new Token(DIVISION);
        case '*':
                return new Token(MULTIPLICATION);
        case '=':
                return new Token(EQUALS);
        case '>':
                return new Token(GREATERTHAN);
        case '<':
                return new Token(LESSTHAN);
        default:
            break;
        }
        break;
    }
    if(isdigit(peek))
    {
        int v = 0;
        do 
        {
            v = 10 * v + (peek - '0');
            stream->get(peek);
        } while(isdigit(peek));
        stream->unget();
        return new Num(v);
    }
    if(isalpha(peek) || peek == '#')
    {
        std::stringbuf b;
        do
        {
            b.sputc(peek);
            stream->get(peek);
        }while (isalnum(peek) || peek == '?');
        stream->unget();
        auto t = table.find(b.str());
        if (t != table.end()) return t->second;
        Word *w = new Word(b.str(), uniqueNum++);
        table[b.str()] = w;
        return w;
    }
    return new Token(ERROR);

}
