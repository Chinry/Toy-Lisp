#include "linked_tree.hpp"
#include <iostream>

void TreeNode::addChild(TreeNode *child)
{
    child->parent = this;
    children.push_back(child);
}

TreeNode::~TreeNode()
{
    for(TreeNode* child : children)
    {
        delete child;
    }
    delete this;
}

TreeNode::TreeNode(Token *d)
{
    data = d;
}

void TreeNode::print()
{
    if(data != nullptr) {std::cout << data->toString() << " ";

    if(data->tag == OPENPAREN)
    {
        std::cout << std::endl;
    }}
    for(TreeNode *child : children)
    {
        child->print();
    }
}
