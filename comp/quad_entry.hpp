#pragma once
#include "token.hpp"
struct Operator
{
    int address;
    enum TokenId id;
    bool isId;
};

struct Argument
{
    int value;
    bool isVariable;
};
struct QuadEntry
{
    Operator op;
    Argument arg1;
    Argument arg2;
    Argument result;
};

