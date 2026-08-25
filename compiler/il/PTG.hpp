// preet
#ifndef PTG_H
#define PTG_H
#include <set>
#include <map>
#include <utility>
#include <vector>
#include <unordered_set>
#include "il/Node.hpp"

// #include "il/SymbolReference.hpp"
// #include "il/Node.hpp"
namespace TR {
class Node;
class SymbolReference;
} // namespace TR

class PTG { // MayPointsTo To Start with
public:
    std::map<int, /* auto slots */
        std::set<TR::Node *> /* set of Nodes */
        >
        _stack;

    std::map<std::pair<TR::Node *, TR::SymbolReference *>, /*{01,f} */
        std::set<TR::Node *> /* set of Nodes */
        >
        _heap;

    PTG();

    /*
    STACK
    */
    void printStack();
    int insertIntoStack(int a, TR::Node *obj);
    bool isKeyPresentInStack(int a);
    std::set<TR::Node *> getNodeSetForKeyInStack(int key);
    bool deletekeyFromFromStack(int a);
    void setPointsToOfKeyInStackToBottom(
        int key); // the "caller already should have done the check" of correct element insertion
    bool isPointsToOfKeyInStackBottom(int key);
    bool isPointsToOfKeyInStackUnknown(int key);

    /*
    HEAP
    */
    void printHeap();
    int insertIntoHeap(std::pair<TR::Node *, TR::SymbolReference *> objField, TR::Node *obj);
    bool isKeyPresentInHeap(std::pair<TR::Node *, TR::SymbolReference *> objField);
    bool deleteKeyFromHeap(std::pair<TR::Node *, TR::SymbolReference *> objField);
    bool isPointsToOfKeyInHeapBottom(std::pair<TR::Node *, TR::SymbolReference *> key);
    void setPointsToOfKeyInHeapToBottom(std::pair<TR::Node *, TR::SymbolReference *>
            key); // the "caller already should have done the check" of correct element insertion
    bool doesStarFieldFromNodeExists(TR::Node *node);

    std::set<TR::Node *> getNodeSetForKeyInHeap(std::pair<TR::Node *, TR::SymbolReference *> key);

    bool equals(PTG *other);
    bool isEmpty();

    std::vector<std::pair<TR::Node *, TR::SymbolReference *> > getHeapKeysWithNode(TR::Node *node);

    void findNodes(TR::Node *node, std::vector<TR::SymbolReference *> fieldStack, int currLevel, int finalLevel,
        std::vector<TR::Node *> &res);
    bool pathExistBetween(TR::Node *src, TR::Node *dest, std::unordered_set<TR::Node *> &visited);
    std::set<TR::Node *> getReachableNeighbours(TR::Node *nodeObj);

    /* "?" node encodeing as follows, for eliminating definition free paths*/
    static TR::Node *unknownNode()
    {
        static TR::Node *dummyNode = TR::Node::create(nullptr, TR::BadILOp, 0);
        return dummyNode;
    }

    // Onull
    static TR::Node *nullConstNode()
    {
        static TR::Node *dummyNullNode =TR::Node::create(TR::lconst, 0, 0);;
        return dummyNullNode;
    }

    static bool isMust(const std::set<TR::Node *> &pointees);
    static TR::Node *mustPointee(const std::set<TR::Node*> &pointees);
};

#endif
