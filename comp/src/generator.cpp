#include "generator.hpp"
#include <iostream>

Generator::Generator(int entries)
{
    varEntries = new std::vector<int>(200);
}




std::vector<QuadEntry> Generator::generate(TreeNode *tree, std::vector<int> params)
{
    treeTops.push(tree);
    currentTree = tree;
    nodeIt = 0;
    for(;;)
    {
        switch(currentTree->children[nodeIt]->data->tag) //on element tag
        {
            case OPENPAREN:
            {
                handleOpenParen();
                break;
            }
            case DEFINE:
            {
                handleDefine();
                break;
            }
            case ADDITION:
            case SUBTRACTION:
            case MULTIPLICATION:
            case DIVISION:
            case MODULO:
            {
                handleIdOp(currentTree->children[nodeIt]->data->tag);
                break;
            }
            default:
            break;
        }
        if(++nodeIt == static_cast<int>(currentTree->children.size()))
        {
            handleListEnd();
        }
        if(currentTree == nullptr) 
        {
            break;
        }

    }
    return entries;
}

void Generator::handleOpenParen()
{
    itStack.push(nodeIt);
    currentTree = currentTree->children[nodeIt];
    nodeIt = 0;
}

void Generator::handleListEnd()
{
    nodeIt = itStack.top();
    itStack.pop();
    currentTree = currentTree->parent;
}

void Generator::handleDefine()
{
    if(currentTree->children[nodeIt + 1]->data->tag == OPENPAREN)
    {
        handleDefineFunc();
    }
    else
    {
        handleDefineVar();
    }
}


void Generator::handleDefineFunc()
{
    std::vector<TreeNode*> parameters = currentTree->children[nodeIt + 1]->children;
}

void Generator::handleDefineVar()
{

}

void Generator::handleIdOp(TokenId op)
{
    std::vector<TreeNode*> currentList = currentTree->children;
    QuadEntry entry;
    entry.op.isId = true;
    entry.op.id = op;
    switch(currentList[1]->data->tag)
    {
        case NUM:
        {
            entry.arg1.isVariable = false;
            entry.arg1.value = (static_cast<Num*>(currentList[1]->data))->value;
            break;
        }
        case ID:
        {
            entry.arg2.isVariable = true;
            entry.arg2.value = (static_cast<Word*>(currentList[1]->data))->identifier;
            break;
        }
        case OPENPAREN:
        {
            break;
        }
        default:
            break;


        
    }
    if (entries.size() == 0)
    {
        entry.result.isVariable = false;
        entry.result.value = 0;
    }
    entries.push_back(entry);
    if (itStack.size() > 0){nodeIt = itStack.top();
    itStack.pop();}
    currentTree = currentTree->parent;

}




void Generator::print()
{
    for(QuadEntry &e : entries)
    {
        if (e.op.isId)
        {
            std::cout << "OP Function At:" << e.op.address;
        }
        else
        {
            std::cout << "OP ID:" << e.op.id;
        }
        std::cout << " arg1:" << e.arg1.value << " arg2:" << e.arg2.value
                << " result:" << e.result.value;
    }
}