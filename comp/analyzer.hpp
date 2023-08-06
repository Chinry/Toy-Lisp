#pragma once

#include "linked_tree.hpp"
class Analyzer
{
public:
    Analyzer(); 
    void append(Token *t);
    TreeNode *tree;
private:
    TreeNode *lastNode;
};