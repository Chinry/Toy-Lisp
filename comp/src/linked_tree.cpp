#include "linked_tree.hpp"
#include <iostream>
#include <stack>

void TreeNode::addChild(TreeNode *child)
{
    child->parent = this;
    children.push_back(child);
}

TreeNode::TreeNode(Token *d)
{
    data = d;
}
TreeNode::TreeNode()
{
    save = false;
}
void TreeNode::TreeNodeFree()
{
    std::stack<TreeNode*> s;
    TreeNode *n;
    s.push(this);
    while(!s.empty())
    {
        n = s.top();
        s.pop();
        for(TreeNode* node : n->children)
            if(!node->save)
                s.push(node);
        
        if(n->data != nullptr)
        {
            switch(n->data->tag)
            {
                case OPENPAREN:
                case CLOSEPAREN:
                case ADDITION:
                case SUBTRACTION:
                case DIVISION:
                case MULTIPLICATION:
                case EQUALS:
                case GREATERTHAN:
                case LESSTHAN:
                case NUM:
                case END:
                    delete n->data;
                    n->data = nullptr;
                    break;
                default:
                    break;
            }
            
        }
        delete n;
    }
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
