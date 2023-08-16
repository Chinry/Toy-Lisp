#include "generator.hpp"
#include <sstream>

Generator::Generator(int entries)
{
    varEntries = std::vector<int>(200);
    funcEntries = std::vector<TreeNode*>(50);
    lastEntry = entries;
    lastFuncEntry = -1;
}

std::string Generator::generate(TreeNode *tree)
{
    if (tree->children.size() == 0)
        return "";
    else
    {
        std::stringstream s;
        for(TreeNode *child : tree->children)
        {
            if (child->children[0]->data->tag == DEFINE)
                if(child->children[1]->data->tag == OPENPAREN)
                    handleDefineFunc(child);
                else
                    handleDefineVar(child->children);
            else
                s << handleNumericalOperation(child->children) << "\n"; 
        }
        return s.str();
    }

}

void Generator::handleDefineVar(std::vector<TreeNode*> items)
{
    int id = (static_cast<Word*>(items[1]->data))->identifier;
    int value = (static_cast<Num*>(items[2]->data))->value;
    varEntries[id] = value;
}

void Generator::handleDefineFunc(TreeNode *item)
{
    lastFuncEntry++;
    funcEntries[lastFuncEntry] = item;
    int id = (static_cast<Word*>(item->children[1]->children[0]->data))->identifier;
    varEntries[id] = lastFuncEntry;
}

int Generator::handleNumericalOperation(std::vector<TreeNode*> items)
{

    std::vector<int> operands;
    for(long unsigned int i = 1; i < items.size(); i++)
    {
        if(items[i]->data->tag == OPENPAREN)
        {
            operands.push_back(handleNumericalOperation(items[i]->children));
        }
        else if(items[i]->data->tag == ID)
        {
            int id = (static_cast<Word*>(items[i]->data))->identifier;
            operands.push_back(varEntries[id]);
        }
        else 
        {
            operands.push_back((static_cast<Num*>(items[i]->data))->value);
        }
    
    }
    
    
    
    switch(items[0]->data->tag)
    {
    case ID:
        copyFunctionParams(items, operands);
        return runStoredFunc(items);
    case ADDITION:
        return operands[0] + operands[1];
    case SUBTRACTION:
        return operands[0] - operands[1];
    case MULTIPLICATION:
        return operands[0] * operands[1];
    case DIVISION:
        return operands[0] / operands[1];
    default:
        return operands[0];
    }
}

void Generator::copyFunctionParams(std::vector<TreeNode*> items, std::vector<int> operands)
{
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    std::vector<TreeNode*> params = funcEntries[funcAddress]->children[1]->children;

    for (long unsigned int i = 1; i < params.size(); i++)
    {
        int varAddress = (static_cast<Word*>(params[i]->data))->identifier;
        varEntries[varAddress] = operands[i - 1];
    }
}
int Generator::runStoredFunc(std::vector<TreeNode*> items)
{
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    return handleNumericalOperation((funcEntries[funcAddress])->children[2]->children);
}