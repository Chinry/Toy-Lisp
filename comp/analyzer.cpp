#include "analyzer.hpp"

Analyzer::Analyzer()
{
    tree = new TreeNode(nullptr);
    lastNode = tree;
}

void Analyzer::append(Token *t)
{
    if(t->tag == OPENPAREN)
    {
        TreeNode *tnode = new TreeNode(t);
        lastNode->addChild(tnode);
        lastNode = tnode;
    }
    if(t->tag == CLOSEPAREN)
    {
        delete t;
        lastNode = lastNode->parent;
    }
    if(t->tag >= NUM)
    {
        TreeNode *tnode = new TreeNode(t);
        lastNode->addChild(tnode);
    }
}