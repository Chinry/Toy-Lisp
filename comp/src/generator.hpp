#include <vector>
#include "quad_entry.hpp"
#include "linked_tree.hpp"
#include <stack>
#include <list>
class Generator
{
public:
    std::vector<QuadEntry> generate(TreeNode *tree, std::vector<int> params);
    Generator(int entries);
    void print();
private:
    std::vector<QuadEntry> entries;
    std::vector<int> *varEntries;
    int lastVarEntry;
    TreeNode* currentTree;
    std::stack<int> itStack;
    std::stack<TreeNode*> treeTops;
    int nodeIt;
    void handleOpenParen();
    void handleListEnd();
    void handleDefine();
    void handleDefineVar();
    void handleDefineFunc();
    void handleIdOp(TokenId op);

};