#include <vector>
#include "linked_tree.hpp"
#include "tagged_result.hpp"
#include <stack>
#include <list>
class Generator
{
public:
    std::string generate(TreeNode *tree);
    Generator(int entries);
private:
    std::stack<std::vector<StackElem>> varStack;
    std::vector<int> varEntries;
    std::vector<TreeNode*> funcEntries;
    std::vector<ConsCell> cells;
    int lastEntry;
    int lastFuncEntry;
    int handleNumericalOperation(std::vector<TreeNode*> items);
    void handleDefineVar(std::vector<TreeNode*> items);
    void handleDefineFunc(TreeNode *items);
    void copyFunctionParams(std::vector<TreeNode*> items, std::vector<int> operands);
    TaggedResult runStoredFunc(std::vector<TreeNode*> items, std::vector<int> operands);
    bool isBoolCheckOp(TokenId tag);
    std::string boolToString(bool b);
    bool handleBoolCheck(std::vector<TreeNode*> items);
    std::vector<int> handleOperands(std::vector<TreeNode*> items);
    TaggedResult runThroughFunc(TreeNode *tree);
    TaggedResult runLine(TreeNode* child);
    int getIdValue(int identifier);
    void setIdValue(int identifier, int value);
    int handleCreateList(std::vector<TreeNode*> items);
    std::string listToString(int cellNum);
};