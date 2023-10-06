
enum ValueTag{IS_INT, IS_POINTER};

struct ConsCell
{
    int next;
    int value;
    enum ValueTag valueType;
    enum ValueTag nextType;
};

enum ResultTag{NUMBER_TAG, BOOLEAN_TAG, LIST_TAG, EMPTY_TAG};

struct TaggedResult
{
    int value;
    ResultTag tag;
};

struct StackElem
{
    int identifier;
    int value;
};

