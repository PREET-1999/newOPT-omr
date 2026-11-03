#ifndef PTGInter_HPP
#define PTGInter_HPP

#include <map>
#include <set>
#include <utility>

#include "AbstractField.hpp"
#include "AbstractNode.hpp"

// All AbstractNode* / AbstractField* stored here must come from an AbstractPool
// (one canonical pointer per value), so plain pointer comparison is correct.
// PTGInter does not own them. Do not modify a node through these pointers.
class PTGInter {
public:
    typedef std::pair<AbstractNode *, AbstractField *> HeapKey;

    // auto slot -> abstract objects.
    // The slot key is only meaningful inside the current compilation;
    // it is never copied into a MethodSummary.
    std::map<int, std::set<AbstractNode *> > _stack;

    // (abstract object, field) -> abstract objects
    std::map<HeapKey, std::set<AbstractNode *> > _heap;

    PTGInter();
    PTGInter(const PTGInter& other);

    /*
    STACK
    */
    void printStack();
    int insertIntoStack(int a, AbstractNode *obj);
    bool isKeyPresentInStack(int a);
    std::set<AbstractNode *> getNodeSetForKeyInStack(int key);
    bool deletekeyFromFromStack(int a);

    /*
    HEAP
    */
    void printHeap();
    int insertIntoHeap(const HeapKey &objField, AbstractNode *obj);
    bool isKeyPresentInHeap(const HeapKey &objField);
    bool deleteKeyFromHeap(const HeapKey &objField);
    std::set<AbstractNode *> getNodeSetForKeyInHeap(const HeapKey &key);

    bool equals(PTGInter *other);
};

#endif // PTGInter_HPP