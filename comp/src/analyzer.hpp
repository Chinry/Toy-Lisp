#pragma once

#include "linked_tree.hpp"
class Analyzer
{
public:
    Analyzer();
    ~Analyzer();
    void append(Token *t);
    TreeNode *tree;
private:
    TreeNode *lastNode;
};