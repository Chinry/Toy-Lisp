enum ResultTag{NUMBER_TAG, BOOLEAN_TAG, EMPTY};

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