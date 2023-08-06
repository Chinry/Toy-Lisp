#include <vector>
#include <fstream>
#include <iostream>
#include "token.hpp"
#include "lexer.hpp"
int main(int argc, char *argv[])
{
    std::fstream f;
    f.open(std::string(argv[1]));
    Lexer l(&f);
    Token t = l.scan();
    std::vector<Token> v;
    while (t.tag != END)
    {
        v.push_back(t);
        t = l.scan();
    }
    return 0;
}
