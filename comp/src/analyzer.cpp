#include "analyzer.hpp"

Analyzer::Analyzer()
{
    tree = new TreeNode(nullptr);
    lastNode = tree;
}

Analyzer::~Analyzer()
{
    tree->TreeNodeFree();
}

void Analyzer::append(Token *t)
{
    if(t->tag == OPENPAREN)
    {
        TreeNode *tnode = new TreeNode(t);
        lastNode->addChild(tnode);
        lastNode = tnode;
    }
    else if(t->tag == CLOSEPAREN)
    {
        delete t;
        lastNode = lastNode->parent;
    }
    else
    {
        TreeNode *tnode = new TreeNode(t);
        lastNode->addChild(tnode);
    }
}