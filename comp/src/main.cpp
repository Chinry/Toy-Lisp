#include <vector>
#include <fstream>
#include <iostream>
#include "token.hpp"
#include "lexer.hpp"
#include "analyzer.hpp"
#include "generator.hpp"

int main(int argc, char *argv[])
{
    std::fstream f;
    f.open(std::string(argv[1]));
    Lexer l(&f);
    Analyzer a;
    Token *t;
    Generator g(0);
    t = l.scan();
    while (t->tag != END)
    {    
        a.append(t);
        t = l.scan();
    }
    std::string output = g.generate(a.tree);
    std::cout << output;
    return 0;
}
