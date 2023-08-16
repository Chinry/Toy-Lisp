
#include "../src/analyzer.hpp"
#include "../src/lexer.hpp"
#include <sstream>
#include <iostream>
#include <CppUTest/TestHarness.h>
TEST_GROUP(AnalyzerTests)
{

};

TEST(AnalyzerTests, nestedParens)
{
    std::stringstream stream;
    stream.str("(define (a b) (+ b b))");
    Lexer l(&stream);
    Analyzer a;
    Token *t;
    t = l.scan();
    while (t->tag != END)
    {    
        a.append(t);
        t = l.scan();
    }
    
    std::vector<TokenId> ids = {OPENPAREN, DEFINE, OPENPAREN, ID, ID, OPENPAREN, ADDITION, ID, ID};
    std::vector<TreeNode*> nodes;
    nodes.push_back(a.tree->children[0]); //0 (
    nodes.push_back(nodes[0]->children[0]); //1 define
    nodes.push_back(nodes[0]->children[1]); //2 (
    nodes.push_back(nodes[2]->children[0]); //3 a
    nodes.push_back(nodes[2]->children[1]); //4 b
    nodes.push_back(nodes[0]->children[2]); //5 (
    nodes.push_back(nodes[5]->children[0]); //6 +
    nodes.push_back(nodes[5]->children[1]); //7 b
    nodes.push_back(nodes[5]->children[2]); //8 b

    CHECK_TRUE(nodes[0]->data->tag == ids[0]);
    CHECK_TRUE(nodes[1]->data->tag == ids[1]);
    CHECK_TRUE(nodes[2]->data->tag == ids[2]);
    CHECK_TRUE(nodes[3]->data->tag == ids[3]);
    CHECK_TRUE(nodes[4]->data->tag == ids[4]);
    CHECK_TRUE(nodes[5]->data->tag == ids[5]);
    CHECK_TRUE(nodes[6]->data->tag == ids[6]);
    CHECK_TRUE(nodes[7]->data->tag == ids[7]);
    CHECK_TRUE(nodes[8]->data->tag == ids[8]);

    CHECK_TRUE(nodes[0]->children.size() == 3);
    CHECK_TRUE(nodes[1]->children.size() == 0);
    CHECK_TRUE(nodes[2]->children.size() == 2);
    CHECK_TRUE(nodes[3]->children.size() == 0);
    CHECK_TRUE(nodes[4]->children.size() == 0);
    CHECK_TRUE(nodes[5]->children.size() == 3);
    CHECK_TRUE(nodes[6]->children.size() == 0);
    CHECK_TRUE(nodes[7]->children.size() == 0);
    CHECK_TRUE(nodes[8]->children.size() == 0);
    
}