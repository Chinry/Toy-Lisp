#include <vector>
#include "linked_tree.hpp"
#include <stack>
#include <list>
class Generator
{
public:
    std::string generate(TreeNode *tree);
    Generator(int entries);
private:
    std::vector<int> varEntries;
    std::vector<TreeNode*> funcEntries;
    int lastEntry;
    int lastFuncEntry;
    int handleNumericalOperation(std::vector<TreeNode*> items);
    void handleDefineVar(std::vector<TreeNode*> items);
    void handleDefineFunc(TreeNode *items);
    void copyFunctionParams(std::vector<TreeNode*> items, std::vector<int> operands);
    int runStoredFunc(std::vector<TreeNode*> items);
};