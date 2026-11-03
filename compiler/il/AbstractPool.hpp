//preet
#ifndef ABSTRACT_POOL_HPP
#define ABSTRACT_POOL_HPP

#include <map>
#include <string>

#include "AbstractField.hpp"
#include "AbstractNode.hpp"

// Hands out ONE canonical AbstractNode* / AbstractField* per distinct value.
// Asking twice for the same (kind, id) returns the same pointer, so pointer
// equality == value equality, and PTGInter can use plain pointer sets/maps.
//
// The pool owns everything it returns. Pointers stay valid until the pool is
// destroyed. Use one pool per compilation, and make sure every PTGInter that
// is compared or merged with another uses the SAME pool.
class AbstractPool {
public:
    AbstractPool() {}

    AbstractNode *getNode(AbstractNode::Kind kind, int id);
    AbstractField *getField(int constantPoolId);

    // Copying would leave PTGs pointing into the old pool.
    AbstractPool(const AbstractPool &) = delete;
    AbstractPool &operator=(const AbstractPool &) = delete;

private:
    // std::map never moves its elements, so the addresses we hand out are stable.
    std::map<AbstractNode, AbstractNode> _nodes;
    std::map<AbstractField, AbstractField> _fields;
};

#endif // ABSTRACT_POOL_HPP