#pragma once
#include <vector>
#include "token.hpp"

struct TreeNode
{
    TreeNode *parent;
    std::vector<TreeNode*> children;
    Token *data;
    bool save;
    TreeNode(Token *d);
    TreeNode();
    void TreeNodeFree();
    void print();
    void addChild(TreeNode *child);
};