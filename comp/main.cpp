#include <vector>
#include <fstream>
#include <iostream>
#include "token.hpp"
#include "lexer.hpp"
#include "analyzer.hpp"

int main(int argc, char *argv[])
{
    std::fstream f;
    f.open(std::string(argv[1]));
    Lexer l(&f);
    Analyzer a;
    Token *t;
    t = l.scan();
    while (t->tag != END)
    {    
        //a.append(t);
        std::cout << t->toString() << " ";
        t = l.scan();
        
    }
    //a.tree->print();
}