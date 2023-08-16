#include "../src/lexer.hpp"
#include <sstream>
#include <vector>

#include <CppUTest/TestHarness.h>

TEST_GROUP(LexerTests)
{

};

TEST(LexerTests, readsCorrectTokens)
{
    std::stringstream stream;
    stream.str("(define (a b) (+ b b))");
    stream.seekp(0);
    Lexer l(&stream);
    Token *t;
    std::vector<TokenId> scanned;
    t = l.scan();
    scanned.push_back(t->tag);
    while (t->tag != END)
    {    
        t = l.scan();
        scanned.push_back(t->tag);
    }
    
    CHECK_TRUE(scanned.size() == 13);
    CHECK_TRUE(scanned[0] == OPENPAREN);
    CHECK_TRUE(scanned[1] == DEFINE);
    CHECK_TRUE(scanned[2] == OPENPAREN);
    CHECK_TRUE(scanned[3] == ID);
    CHECK_TRUE(scanned[4] == ID);
    CHECK_TRUE(scanned[5] == CLOSEPAREN);
    CHECK_TRUE(scanned[6] == OPENPAREN);
    CHECK_TRUE(scanned[7] == ADDITION);
    CHECK_TRUE(scanned[8] == ID);
    CHECK_TRUE(scanned[9] == ID);
    CHECK_TRUE(scanned[10] == CLOSEPAREN);
    CHECK_TRUE(scanned[11] == CLOSEPAREN);
    CHECK_TRUE(scanned[12] == END);
}

