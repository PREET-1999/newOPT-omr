#include "PTGInter.hpp"

#include <iostream>

PTGInter::PTGInter() {}

PTGInter::PTGInter(const PTGInter& other)
    : _stack(other._stack),
      _heap(other._heap)
{
}
static void printNodeSet(const std::set<AbstractNode *> &nodes) {
    std::cout << "{";
    bool first = true;
    for (std::set<AbstractNode *>::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
        if (!first) std::cout << ", ";
        std::cout << (*it)->toString();
        first = false;
    }
    std::cout << "}";
}

/*
STACK
*/

void PTGInter::printStack() {
    std::cout << "STACK:\n";
    if (_stack.empty()) std::cout << "  (empty)\n";
    for (std::map<int, std::set<AbstractNode *> >::const_iterator it = _stack.begin();
         it != _stack.end(); ++it) {
        std::cout << "  slot " << it->first << " -> ";
        printNodeSet(it->second);
        std::cout << "\n";
    }
}

// Returns 1 if the node was newly added, 0 if it was already present.
int PTGInter::insertIntoStack(int a, AbstractNode *obj) {
    return _stack[a].insert(obj).second ? 1 : 0;
}

bool PTGInter::isKeyPresentInStack(int a) {
    return _stack.find(a) != _stack.end();
}

std::set<AbstractNode *> PTGInter::getNodeSetForKeyInStack(int key) {
    std::map<int, std::set<AbstractNode *> >::const_iterator it = _stack.find(key);
    if (it == _stack.end()) return std::set<AbstractNode *>();
    return it->second;
}

bool PTGInter::deletekeyFromFromStack(int a) {
    return _stack.erase(a) > 0;
}

/*
HEAP
*/

void PTGInter::printHeap() {
    std::cout << "HEAP:\n";
    if (_heap.empty()) std::cout << "  (empty)\n";
    for (std::map<HeapKey, std::set<AbstractNode *> >::const_iterator it = _heap.begin();
         it != _heap.end(); ++it) {
        std::cout << "  " << it->first.first->toString() << " --"
                  << it->first.second->toString() << "--> ";
        printNodeSet(it->second);
        std::cout << "\n";
    }
}

// Returns 1 if the node was newly added, 0 if it was already present.
int PTGInter::insertIntoHeap(const HeapKey &objField, AbstractNode *obj) {
    return _heap[objField].insert(obj).second ? 1 : 0;
}

bool PTGInter::isKeyPresentInHeap(const HeapKey &objField) {
    return _heap.find(objField) != _heap.end();
}

bool PTGInter::deleteKeyFromHeap(const HeapKey &objField) {
    return _heap.erase(objField) > 0;
}

std::set<AbstractNode *> PTGInter::getNodeSetForKeyInHeap(const HeapKey &key) {
    std::map<HeapKey, std::set<AbstractNode *> >::const_iterator it = _heap.find(key);
    if (it == _heap.end()) return std::set<AbstractNode *>();
    return it->second;
}

// Valid because every pointer comes from the same AbstractPool,
// so equal values are always the same pointer.
bool PTGInter::equals(PTGInter *other) {
    if (!other) return false;
    return _stack == other->_stack && _heap == other->_heap;
}