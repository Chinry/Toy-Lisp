#pragma once
#include <string>

enum TokenId {NUM = 0, ID, OPENPAREN, CLOSEPAREN, ADDITION, SUBTRACTION,
	MULTIPLICATION, DIVISION, END, LAMBDA, LET, DEFINE, COND, TRUE, FALSE,
	CONS, CAR, CDR, LIST, EQUALS, ISNULL, QUOTE, MODULO};

struct Token
{
	enum TokenId tag;
	Token(enum TokenId t);
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
