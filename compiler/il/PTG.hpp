//preet
#ifndef PTG_H
#define PTG_H
#include <set>
#include <map>
#include <utility>
#include <vector>

// #include "il/SymbolReference.hpp"
// #include "il/Node.hpp"

namespace TR {
    class Node;
    class SymbolReference;
}
class PTG
{ // MayPointsTo To Start with
public:
  // stack a->{0xaabcde, 0xabcde}
  std::map<int,        /* auto slots */
      std::set<TR::Node *> /* set of Nodes */
      >
      _stack;

  std::map<std::pair<TR::Node *, TR::SymbolReference *>, /*{01,f} */
      std::set<TR::Node *>                     /* set of Nodes */
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
    void setPointsToOfKeyInStackToBottom(int key); //the "caller already should have done the check" of correct element insertion
    bool isPointsToOfKeyInStackBottom(int key);

  /*
  HEAP
  */
  void printHeap();
  int insertIntoHeap(std::pair<TR::Node*,TR::SymbolReference*> objField ,TR::Node *obj);
    bool isKeyPresentInHeap(std::pair<TR::Node*,TR::SymbolReference*> objField);
    bool isPointsToOfKeyInHeapBottom(std::pair<TR::Node*,TR::SymbolReference*> key);
    void setPointsToOfKeyInHeapToBottom(std::pair<TR::Node*,TR::SymbolReference*> key); //the "caller already should have done the check" of correct element insertion

  std::set<TR::Node *> getNodeSetForKeyInHeap(std::pair<TR::Node*,TR::SymbolReference*> key);

  bool equals(PTG* other);
  bool isEmpty();

   std::vector<std::pair<TR::Node*, TR::SymbolReference*>> getHeapKeysWithNode(TR::Node* node);

};

#endif