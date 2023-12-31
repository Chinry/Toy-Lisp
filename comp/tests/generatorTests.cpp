
#include "../src/generator.hpp"
#include "../src/analyzer.hpp"
#include "../src/lexer.hpp"
#include <sstream>
#include <iostream>
#include <CppUTest/TestHarness.h>

TEST_GROUP(GeneratorOpTest)
{
   
};

TEST(GeneratorOpTest, nothingDoNothing)
{
   Generator g(0);
   TreeNode node;
   node.data = nullptr;
   std::string result = g.generate(&node);
   CHECK(result == "");
}

TEST(GeneratorOpTest, simpleAddition)
{
   Generator g(0);
   std::vector<Num>tokens(4);
   std::vector<TreeNode> nodes(5);
   nodes[0].data = nullptr;
   nodes[1].data = &tokens[0];
   nodes[2].data = &tokens[1];
   nodes[3].data = &tokens[2];
   nodes[4].data = &tokens[3];
   tokens[0].tag = OPENPAREN;
   tokens[1].tag = ADDITION;
   tokens[2].tag = NUM;
   tokens[3].tag = NUM;
   tokens[2].value = 2;
   tokens[3].value = 3;
   nodes[0].addChild(&nodes[1]);
   nodes[1].addChild(&nodes[2]);
   nodes[1].addChild(&nodes[3]);
   nodes[1].addChild(&nodes[4]);

   std::string result = g.generate(&nodes[0]);

   CHECK(result == "5\n");
}

TEST(GeneratorOpTest, simpleSubtraction)
{
   Generator g(0);
   std::vector<Num>tokens(4);
   std::vector<TreeNode> nodes(5);
   nodes[0].data = nullptr;
   nodes[1].data = &tokens[0];
   nodes[2].data = &tokens[1];
   nodes[3].data = &tokens[2];
   nodes[4].data = &tokens[3];
   tokens[0].tag = OPENPAREN;
   tokens[1].tag = SUBTRACTION;
   tokens[2].tag = NUM;
   tokens[3].tag = NUM;
   tokens[2].value = 5;
   tokens[3].value = 3;
   nodes[0].addChild(&nodes[1]);
   nodes[1].addChild(&nodes[2]);
   nodes[1].addChild(&nodes[3]);
   nodes[1].addChild(&nodes[4]);

   std::string result = g.generate(&nodes[0]);

   CHECK(result == "2\n");
}

TEST(GeneratorOpTest, multipleAtOnce)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(/ 4 2) (* 5 6) (+ 2 4)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "2\n30\n6\n");
}

TEST(GeneratorOpTest, nestedOperations)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(* (/ (* 7 8) 7) (* 2 2))");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "32\n");
}

TEST(GeneratorOpTest, defineVarAndUse)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(define a 5) (define b 6) (* a b)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "30\n");
}

TEST(GeneratorOpTest, defineFuncAndUse)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(define (func a b) (* a b)) (func 1 2)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "2\n");
}

TEST(GeneratorOpTest, defineFuncAndUseManyArgs)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(define (func a b c) (* a (* b c))) (func 3 2 3)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "18\n");
}

TEST(GeneratorOpTest, canRunEquals)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(define n 3) (= n 3) (> n 3) (< n 4)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "#t\n#f\n#t\n");
}

TEST(GeneratorOpTest, numberReturnsnumber)
{
   Generator g(0);
   std::stringstream stream;
   stream.str(" 1 ");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "1\n");
}

TEST(GeneratorOpTest, ifstatementruns)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(if (= 3 3) 1 2)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "1\n");
}

TEST(GeneratorOpTest, recursion)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(define (factorial n) (if (= n 1) 1 (* n (factorial (- n 1)))))(factorial 5) ");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "120\n");
}

TEST(GeneratorOpTest, truePrintBack)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("#t ");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "#t\n");
}
TEST(GeneratorOpTest, list_create)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(list 1 2 3 4)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "(1 2 3 4)\n");
}
TEST(GeneratorOpTest, list_create_nested)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(list 1 (list 1 2 3) (list 1 2 (list 1 2)) 4)");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "(1 (1 2 3) (1 2 (1 2)) 4)\n");
}

TEST(GeneratorOpTest, run_car)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(car (list 1 2 3))");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "1\n");
}

TEST(GeneratorOpTest, runCarOfList)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(car (list (list 1 2) 2 3))");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "(1 2)\n");
}

TEST(GeneratorOpTest, runcdr)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("(cdr (list 1 2 3))");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "(2 3)\n");
}

TEST(GeneratorOpTest, runNullCheck)
{
   Generator g(0);
   std::stringstream stream;
   stream.str("( null? (list 1 2 3))");
   Lexer l(&stream);
   Analyzer a;
   Token *t;
   t = l.scan();
   while (t->tag != END)
   {    
        a.append(t);
       t = l.scan();
   }
   std::string result = g.generate(a.tree);

   CHECK(result == "#f\n");
}
