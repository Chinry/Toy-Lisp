#include "generator.hpp"
#include <sstream>

Generator::Generator(int entries)
{
    varEntries = std::vector<int>(200);
    funcEntries = std::vector<TreeNode*>(50);
    lastEntry = entries;
    lastFuncEntry = -1;
    varStack.push(std::vector<StackElem>());
    cells.push_back(ConsCell());
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
                case LIST_TAG:
                    s << listToString(r.value) << "\n";
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

std::string Generator::listToString(int cellNum)
{
    std::stringstream stream;
    stream << "(";
    while(cellNum != 0)
    {
        if(cells[cellNum].valueType == IS_INT)
        {
            stream << cells[cellNum].value;
        }
        else
        {
            stream << listToString(cells[cellNum].value);
        }
        cellNum = cells[cellNum].next;
        if(cellNum != 0)
        {
            stream << " ";
        }
    }
    stream << ")";
    return stream.str();
    
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
    setIdValue(id, value);
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
            operands.push_back(getIdValue(id));
        }
        else 
        {
            operands.push_back((static_cast<Num*>(items[i]->data))->value);
        }
    
    }
    return operands;
}


int Generator::getIdValue(int identifier)
{
    for(long unsigned int i = 0; varStack.top().size() > i; i++)
    {
        if(varStack.top()[i].identifier == identifier)
            return varStack.top()[i].value;
    }
    return varEntries[identifier];
}

void Generator::setIdValue(int identifier, int value)
{
    for(long unsigned int i = 0; varStack.top().size() > i; i++)
    {
        if(varStack.top()[i].identifier == identifier)
        {
            varStack.top()[i].value = value;
            return;
        }
    }
    varEntries[identifier] = value;
}

int Generator::handleNumericalOperation(std::vector<TreeNode*> items)
{

    std::vector<int> operands = handleOperands(items);
    
    
    switch(items[0]->data->tag)
    {
    case ID:
        return runStoredFunc(items,operands).value;
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
        StackElem elem;
        elem.value = operands[i - 1];
        elem.identifier = (static_cast<Word*>(params[i]->data))->identifier;
        varStack.top().push_back(elem);
    }
}

TaggedResult Generator::runStoredFunc(std::vector<TreeNode*> items, std::vector<int> operands)
{
    varStack.push(std::vector<StackElem>());
    copyFunctionParams(items, operands);
    int funcAddress = varEntries[(static_cast<Word*>(items[0]->data))->identifier];
    return runThroughFunc((funcEntries[funcAddress])->children[2]);
    varStack.pop();
}

TaggedResult Generator::runThroughFunc(TreeNode *tree)
{
    TaggedResult r;
    r.tag = EMPTY_TAG;
    if(tree->children.size() > 0 && tree->children[0]->data->tag == OPENPAREN)
    {
        for(TreeNode *child : tree->children)
        {
            TaggedResult res = runLine(child);
            if(res.tag != EMPTY_TAG)
            {
                r = res;
            }
        }
    }
    else 
    {
        TaggedResult res = runLine(tree);
        if(res.tag != EMPTY_TAG)
        {
            r = res;
        }
    }
    return r;
}

TaggedResult Generator::runLine(TreeNode* child)
{
    TaggedResult r;
    if (child->data->tag == NUM)
    {
        r.value = static_cast<Num*>(child->data)->value;
        r.tag = NUMBER_TAG;
    }
    else if (child->data->tag == FALSE || child->data->tag == TRUE)
    {
        if(child->data->tag == FALSE)
        {
            r.value = false;
        }
        else
        {
            r.value = true;
        }
        r.tag = BOOLEAN_TAG;
    }
    else if (child->children[0]->data->tag == IF)
    {
        r = runThroughFunc(child->children[1]);
        if (r.value)
        {
            r = runThroughFunc(child->children[2]);
        }
        else 
        {
            r = runThroughFunc(child->children[3]);
        }
    }
    else if (child->children[0]->data->tag == DEFINE)
    {
        if(child->children[1]->data->tag == OPENPAREN)
            handleDefineFunc(child);
        else
            handleDefineVar(child->children);
        r.tag = EMPTY_TAG;
    }
    else if (child->children[0]->data->tag == CAR)
    {
        int value = runLine(child->children[1]).value;
        if (cells[value].valueType == IS_INT)
            r.tag = NUMBER_TAG;
        else
            r.tag = LIST_TAG;
        r.value = cells[value].value;
    }
    else if (child->children[0]->data->tag == ISNULL)
    {
        r.tag = BOOLEAN_TAG;
        TaggedResult listResult = runLine(child->children[1]);
        if (listResult.tag == EMPTY_TAG)
            r.value = true;
        else
            r.value = false;
        
    }
    else if (child->children[0]->data->tag == CDR)
    {
        int value = runLine(child->children[1]).value;
        if (cells[value].next == 0)
            r.tag = EMPTY_TAG;
        else
        {
            r.tag = LIST_TAG;
            r.value = cells[value].next;
        }
    }
    else if (child->children[0]->data->tag == LIST)
    {
        r.tag = LIST_TAG;
        r.value = handleCreateList(child->children);
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

int Generator::handleCreateList(std::vector<TreeNode*> items)
{
    cells.push_back(ConsCell());
    int retcell = cells.size() - 1;
    int lastCell = retcell;
    cells[retcell].nextType = IS_POINTER;
    cells[retcell].valueType = IS_INT;
    cells[retcell].next = 0;
    if(items[1]->data->tag == OPENPAREN)
    {
        cells[retcell].valueType = IS_POINTER;
        cells[retcell].value = handleCreateList(items[1]->children);
    }
    else
    {
        cells[retcell].value = static_cast<Num*>(items[1]->data)->value;
    }
    for(unsigned long int i = 2; i < items.size(); i++)
    {
        cells.push_back(ConsCell());
        int cellNum = cells.size() - 1;
        if(items[i]->data->tag == OPENPAREN)
        {
            cells[cellNum].valueType = IS_POINTER;
            cells[cellNum].value = handleCreateList(items[i]->children);
        }
        else
        {
            cells[cellNum].value = static_cast<Num*>(items[i]->data)->value;
            cells[cellNum].valueType = IS_INT;
        }
        cells[cellNum].next = 0;
        cells[cellNum].nextType = IS_POINTER;
        cells[lastCell].next = cellNum;
        lastCell = cellNum;
    }
    return retcell;
}
