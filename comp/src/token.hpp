#pragma once
#include <string>

enum TokenId {OPENPAREN = 0, CLOSEPAREN, NUM, ID, ADDITION, SUBTRACTION,
	MULTIPLICATION, DIVISION, END, LAMBDA, LET, DEFINE, COND, TRUE, FALSE,
	CONS, CAR, CDR, LIST, EQUALS, ISNULL, QUOTE, MODULO, ERROR};



struct Token
{
	enum TokenId tag;
	Token(enum TokenId t);
	Token(void);
	std::string toString();
};

struct Num : Token
{
	int value;
	Num(int val);
	Num();
};

struct Word : Token
{
	std::string lexeme;
	int identifier;
	Word(std::string lex, int id);
	Word(std::string lex, TokenId t);
};
