#pragma once
#include <string>

enum TokenId {OPENPAREN = 0, CLOSEPAREN, NUM, ID, ADDITION, SUBTRACTION,
	MULTIPLICATION, DIVISION, END, LAMBDA, LET, DEFINE, COND, TRUE, FALSE,
	CONS, CAR, CDR, LIST, EQUALS, ISNULL, QUOTE, MODULO, ERROR};



struct Token
{
	enum TokenId tag;
	Token(enum TokenId t);
	std::string toString();
};

struct Num : Token
{
	int value;
	Num(int val);
};

struct Word : Token
{
	std::string lexeme;
	Word(std::string lex);
	Word(std::string lex, TokenId t);
};
