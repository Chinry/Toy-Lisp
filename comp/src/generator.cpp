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

    int operand1, operand2;
    
    if(items[1]->data->tag == OPENPAREN)
    {
        operand1 = handleNumericalOperation(items[1]->children);
    }
    else if(items[1]->data->tag == ID)
    {
        int id = (static_cast<Word*>(items[1]->data))->identifier;
        operand1 = varEntries[id];
    }
    else 
    {
        operand1 = (static_cast<Num*>(items[1]->data))->value;
    }
    
    
    if(items[2]->data->tag == OPENPAREN)
    {
        operand2 = handleNumericalOperation(items[2]->children);
    }
    else if(items[2]->data->tag == ID)
    {
        int id = (static_cast<Word*>(items[2]->data))->identifier;
        operand2 = varEntries[id];
    }
    else 
    {
        operand2 = (static_cast<Num*>(items[2]->data))->value;
    }
    
    switch(items[0]->data->tag)
    {
    case ID:
        copyFunctionParams(items, operand1, operand2);
        return runStoredFunc(items);
    case ADDITION:
        return operand1 + operand2;
    case SUBTRACTION:
        return operand1 - operand2;
    case MULTIPLICATION:
        return operand1 * operand2;
    case DIVISION:
        return operand1 / operand2;
    default:
        return operand1;
    }
}

void Generator::copyFunctionParams(std::vector<TreeNode*> items, int operand1, int operand2)
{
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    int var1Address = (static_cast<Word*>(funcEntries[funcAddress]->children[1]->children[1]->data))->identifier;
    int var2Address = (static_cast<Word*>(funcEntries[funcAddress]->children[1]->children[2]->data))->identifier;
    varEntries[var1Address] = operand1;
    varEntries[var2Address] = operand2;
}
int Generator::runStoredFunc(std::vector<TreeNode*> items)
{
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    return handleNumericalOperation((funcEntries[funcAddress])->children[2]->children);
}