// preet
#include <iostream>
#include "il/PTG.hpp"
#include "il/SymbolReference.hpp"
#include "PTG.hpp"

// using namespace std;

PTG::PTG()
{
    // cout<<"PTG constructor mein kuch kro\n";
}

/* stack a->{0xaabcde, 0xabcde}
map < int,               // auto slots //
    set<TR::Node *>          // set of Nodes //
    > _stack;  */
void PTG::printStack()
{
    std::cout << "STACK : ";
    std::map<int, std::set<TR::Node *> >::iterator it;
    std::set<TR::Node *>::iterator nodeIt;
    // for (it = _stack.begin(); it != _stack.end(); ++it) {
    //     cout<<" " <<it->first <<"-> { ";
    //     for(nodeIt=it->second.begin();nodeIt!=it->second.end();nodeIt++){
    //         cout<<(*nodeIt)->_id <<" ";
    //     }
    //     cout<<" }\n";
    // }
    //     cout<<"\n";

    for (it = _stack.begin(); it != _stack.end(); ++it) {
        std::cout << " " << it->first << "-> { ";

        if (isPointsToOfKeyInStackBottom(it->first)) {
            std::cout << "_|_";
        } else {
            for (nodeIt = it->second.begin(); nodeIt != it->second.end(); nodeIt++) {
                std::cout << *nodeIt << " ";
            }
        }

        std::cout << " }\n";
    }
    std::cout << "\n";
}

bool PTG::isKeyPresentInStack(int key)
{
    std::map<int, std::set<TR::Node *> >::iterator it;
    for (it = _stack.begin(); it != _stack.end(); ++it) {
        if (it->first == key) { // found key
            return true;
        }
    }
    return false;
}

std::set<TR::Node *> PTG::getNodeSetForKeyInStack(int key)
{
    std::set<TR::Node *> nodeSet;
    std::map<int, std::set<TR::Node *> >::iterator it;
    for (it = _stack.begin(); it != _stack.end(); ++it) {
        if (it->first == key) { // found key
            return it->second; // return the value (Set<TR::Node *>)
        }
    }
    return nodeSet; // check if empty set is returned correcly to caller:TODO
}

int PTG::insertIntoStack(int a, TR::Node *obj)
{
    std::cout << "inserting into stack key : " << a << "\n";
    std::set<TR::Node *> nodes;
    if (PTG::isKeyPresentInStack(a)) {
        nodes = getNodeSetForKeyInStack(a);

        // insert the obj into the existing nodes set.
        nodes.insert(obj);
        // _stack.insert(pair<int,set<TR::Node *>>(a,nodes)); //doesnt update as key is already present

        // first erase , then add( is there a better way?)
        _stack.erase(a);
        _stack.insert(std::pair<int, std::set<TR::Node *> >(a, nodes));
    } else {
        nodes.insert(obj);

        // create entry in _stack
        _stack.insert(std::pair<int, std::set<TR::Node *> >(a, nodes));
    }

    return 1;
}

bool PTG::deletekeyFromFromStack(int a)
{
    if (PTG::isKeyPresentInStack(a)) {
        _stack.erase(a);
        return true;
    }
    return false;
}

//   map<pair<TR::Node *, TR::SymbolReference *>,
//       set<TR::Node *>
//       >
//       _heap;
void PTG::printHeap()
{
    std::cout << "HEAP : ";
    std::map<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *> >::iterator it;
    std::set<TR::Node *>::iterator nodeIt;

    std::pair<TR::Node *, TR::SymbolReference *> objectFieldPair;
    for (it = _heap.begin(); it != _heap.end(); ++it) {
        objectFieldPair = it->first;
        std::cout << "[" << objectFieldPair.first << "." << (objectFieldPair.second)->getCPIndex() << "]-> { ";

        if (isPointsToOfKeyInHeapBottom(objectFieldPair)) {
            std::cout << "_|_";
        }

        else {
            for (nodeIt = it->second.begin(); nodeIt != it->second.end(); nodeIt++) {
                std::cout << (*nodeIt) << " ";
            }
        }
        std::cout << " }\n";
    }
    std::cout << " \n";
}

int PTG::insertIntoHeap(std::pair<TR::Node *, TR::SymbolReference *> objField, TR::Node *obj)
{
    std::cout << "fieldAddress " << &objField << "\n";
    // int nodeId = objField.first->_id;
    // const char* fieldName = objField.second->_fieldName;
    //     cout<<"inserting into heap key : " << nodeId <<"." <<fieldName <<"\n";

    std::set<TR::Node *> nodes;

    // key present;
    if (isKeyPresentInHeap(objField)) {
        // cout<<"got field\n";
        nodes = _heap.find(objField)->second;
        nodes.insert(obj);

        // first erase , then add( is there a better way?)
        _heap.erase(objField);
        _heap.insert(std::pair<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *> >(objField, nodes));

    } else {
        nodes.insert(obj);

        _heap.insert(std::pair<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *> >(objField, nodes));
    }
    return 0;
}

std::set<TR::Node *> PTG::getNodeSetForKeyInHeap(std::pair<TR::Node *, TR::SymbolReference *> key)
{
    auto it = _heap.find(key);
    if (it != _heap.end()) {
        return it->second;
    }
    return std::set<TR::Node *>();
}

bool PTG::isKeyPresentInHeap(std::pair<TR::Node *, TR::SymbolReference *> objField)
{
    if (_heap.find(objField) != _heap.end())
        return true;
    return false;
}

bool PTG::isPointsToOfKeyInHeapBottom(std::pair<TR::Node *, TR::SymbolReference *> key)
{
    std::set<TR::Node *> nodeSet = getNodeSetForKeyInHeap(key);
    if (nodeSet.size() == 1) {
        if (*(nodeSet.begin()) == nullptr)
            return true;
    }
    return false;
}

void PTG::setPointsToOfKeyInHeapToBottom(std::pair<TR::Node *, TR::SymbolReference *> objField)
{
    std::set<TR::Node *> nodes;
    TR::Node *bottom = nullptr;
    nodes.insert(bottom); // no need to append, just replace
    _heap.insert(std::pair<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *> >(objField, nodes));

    // // key present; (if you think caller is not checking properly, uncomment in future)
    // if (isKeyPresentInHeap(objField))
    // {
    //     nodes.insert(bottom); //no need to append, just replace

    //     // first erase , then add( is there a better way?)
    //     _heap.erase(objField);
    //     _heap.insert(std::pair<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *>>(objField, nodes));
    // }
    // // else
    // // {
    // //     nodes.insert(bottom);
    // //     _heap.insert(pair<pair<Node *, SymbolReference *>, set<Node *>>(objField, nodes));
    // // }
}

void PTG::setPointsToOfKeyInStackToBottom(int autoSlot)
{
    std::set<TR::Node *> nodes;
    TR::Node *bottom = nullptr;
    nodes.insert(bottom); // no need to append, just replace
    _stack.insert(std::pair<int, std::set<TR::Node *> >(autoSlot, nodes));

    // // key present; (if you think caller is not checking properly, uncomment in future)
    // if (isKeyPresentInHeap(objField))
    // {
    //     nodes.insert(bottom); //no need to append, just replace

    //     // first erase , then add( is there a better way?)
    //     _heap.erase(objField);
    //     _heap.insert(std::pair<std::pair<TR::Node *, TR::SymbolReference *>, std::set<TR::Node *>>(objField, nodes));
    // }
    // // else
    // // {
    // //     nodes.insert(bottom);
    // //     _heap.insert(pair<pair<Node *, SymbolReference *>, set<Node *>>(objField, nodes));
    // // }
}

bool PTG::isPointsToOfKeyInStackBottom(int key)
{
    std::set<TR::Node *> nodeSet = getNodeSetForKeyInStack(key);
    if (nodeSet.size() == 1) {
        if (*(nodeSet.begin()) == nullptr)
            return true;
    }
    return false;
}

bool PTG::equals(PTG *another)
{
    // std::cout << "in equals \n";
    PTG *one = this;
    if (one->isEmpty() && another->isEmpty())
        return true;

    /*
    STACK
    */
    auto stackAnother = another->_stack;
    auto stackOne = one->_stack;

    // check sizes
    if (stackOne.size() != stackAnother.size())
        return false;

    // check if keys are same
    for (auto stackOnePair : stackOne) {
        int autoSlot = stackOnePair.first;
        // check if this key is also there in the another Stack
        auto stackAnother = another->_stack;
        auto anotherIt = stackAnother.find(autoSlot);
        if (anotherIt != stackAnother.end()) {
            // key is also in another stack

            // check if nodeSet pointed to by key is same
            std::set<TR::Node *> stackOneNodeSet = stackOnePair.second;
            std::set<TR::Node *> stackAnotherNodeSet = anotherIt->second;
            if (stackOneNodeSet != stackAnotherNodeSet)
                return false;
        } else {
            return false;
        }
    }

    /*
    HEAP
    */
    auto heapOne = one->_heap;
    auto heapAnother = another->_heap;

    // check sizes
    if (heapOne.size() != heapAnother.size())
        return false;
    // traverse through first heap
    for (auto heapOnePair : heapOne) {
        auto objFieldPairOne = heapOnePair.first;
        auto heapAnotherIt = heapAnother.find(objFieldPairOne);
        if (heapAnotherIt != heapAnother.end()) {
            std::set<TR::Node *> heapOneNodeSet = heapOnePair.second;
            std::set<TR::Node *> heapAnotherNodeSet = heapAnotherIt->second;
            if (heapOneNodeSet != heapAnotherNodeSet)
                return false;
        } else {
            return false;
        }
    }

    return true;
}

bool PTG::isEmpty()
{
    if (_stack.empty() && _heap.empty())
        return true;
    return false;
}

std::vector<std::pair<TR::Node *, TR::SymbolReference *> > PTG::getHeapKeysWithNode(TR::Node *node)
{
    std::vector<std::pair<TR::Node *, TR::SymbolReference *> > heapKeys {};
    std::cout << "In getHeapKeysWithNode with (" << node << ")\n";
    for (const auto &heapElement : _heap) {
        std::pair<TR::Node *, TR::SymbolReference *> heapKeyPair = heapElement.first;
        std::cout << "checking... " << heapKeyPair.first << "." << heapKeyPair.second << "\n";

        if (node == heapKeyPair.first) {
            heapKeys.push_back(heapKeyPair);
        }
    }

    return heapKeys;
}
