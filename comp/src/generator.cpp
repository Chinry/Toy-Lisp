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
        TaggedResult r;
        for(TreeNode *child : tree->children)
        {
            r = runLine(child);
            switch(r.tag)
            {
                case NUMBER_TAG:
                    s << r.value << "\n";
                    break;
                case BOOLEAN_TAG:
                    s << boolToString(r.value) << "\n";
                    break;
                default:
                    break;
            }
        }
        return s.str();
    }

}







bool Generator::handleBoolCheck(std::vector<TreeNode*> items)
{
   std::vector<int> operands = handleOperands(items);
   switch(items[0]->data->tag)
    {
    case GREATERTHAN:
        return operands[0] > operands[1];
    case EQUALS:
        return operands[0] == operands[1];
    case LESSTHAN:
        return operands[0] < operands[1];
    default:
        return true;
    }
}

std::string Generator::boolToString(bool b)
{
    if(b) return "#t";
    else return "#f";
}

bool Generator::isBoolCheckOp(TokenId tag)
{
    if(tag == EQUALS || tag == GREATERTHAN || tag == LESSTHAN)
        return true;
    return false;
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

std::vector<int> Generator::handleOperands(std::vector<TreeNode*> items)
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
    return operands;
}


int Generator::handleNumericalOperation(std::vector<TreeNode*> items)
{

    std::vector<int> operands = handleOperands(items);
    
    
    switch(items[0]->data->tag)
    {
    case ID:
        copyFunctionParams(items, operands);
        return runStoredFunc(items).value;
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
TaggedResult Generator::runStoredFunc(std::vector<TreeNode*> items)
{
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    return runThroughFunc((funcEntries[funcAddress])->children[2]);
}

TaggedResult Generator::runThroughFunc(TreeNode *tree)
{
    TaggedResult r;
    r.tag = EMPTY;
    if(tree->children[0]->data->tag == OPENPAREN)
    {
        for(TreeNode *child : tree->children)
        {
            TaggedResult res = runLine(child);
            if(res.tag != EMPTY)
            {
                r = res;
            }
        }
    }
    else 
    {
        TaggedResult res = runLine(tree);
        if(res.tag != EMPTY)
        {
            r = res;
        }
    }
    return r;
}

TaggedResult Generator::runLine(TreeNode* child)
{
    TaggedResult r;
    if (child->children[0]->data->tag == DEFINE)
    {
        if(child->children[1]->data->tag == OPENPAREN)
            handleDefineFunc(child);
        else
            handleDefineVar(child->children);
        r.tag = EMPTY;
    }
    else if (isBoolCheckOp(child->children[0]->data->tag))
    {
        r.value = handleBoolCheck(child->children);
        r.tag = BOOLEAN_TAG;
    }
    else
    {
        r.value = handleNumericalOperation(child->children);
        r.tag = NUMBER_TAG;
    }
    return r;
}
