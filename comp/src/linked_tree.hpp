#pragma once
#include <vector>
#include "token.hpp"

struct TreeNode
{
    TreeNode *parent;
    std::vector<TreeNode*> children;
    Token *data;
    TreeNode(Token *d);
    TreeNode();
    void print();
    void addChild(TreeNode *child);
};