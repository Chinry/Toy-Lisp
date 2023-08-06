#pragma once
#include <list>
#include "token.hpp"

struct TreeNode
{
    TreeNode *parent;
    std::list<TreeNode*> children;
    Token *data;
    TreeNode(Token *d);
    void print();
    void addChild(TreeNode *child);
};