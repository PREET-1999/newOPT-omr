#ifndef ABSTRACT_POOL_HPP
#define ABSTRACT_POOL_HPP

#include <map>
#include <utility>

#include "AbstractField.hpp"
#include "AbstractNode.hpp"

// Hands out ONE canonical AbstractNode* / AbstractField* per distinct value.
// Asking twice for the same value returns the same pointer, so pointer
// equality == value equality, and PTGInter can use plain pointer sets/maps.
//
// The pool owns everything it returns. Pointers stay valid until the pool is
// destroyed. Use one pool per compilation (per method analysis), and make sure
// every PTGInter that is compared or merged uses the SAME pool.
// NOT thread-safe.
class AbstractPool {
public:
    typedef std::pair<AbstractNode, AbstractField> LoadOrigin; // (base node, field)

    AbstractPool() : _nextLoadedObjectId(0) {}

    // Canonical node for (kind, ownerMethodId, id).
    AbstractNode *getNode(AbstractNode::Kind kind, int ownerMethodId, int id);

    // Canonical field for a field id.
    AbstractField *getField(int fieldId);

    // The node standing for "the object that base.field pointed to at method entry".
    // The same (base, field) always gives the same node, so re-running a load on the
    // next fixpoint pass does not create a new node. The origin is recorded and can
    // be read back with getLoadOrigins().
    AbstractNode *getLoadedObject(AbstractNode *base, AbstractField *field, int ownerMethodId);

    // LoadedObject node -> (base, field) it was loaded from.
    // Copy this into MethodSummary::loadOrigin when building the summary.
    const std::map<AbstractNode, LoadOrigin> &getLoadOrigins() const { return _loadOrigins; }

    // Copying would leave PTGs pointing into the old pool.
    AbstractPool(const AbstractPool &) = delete;
    AbstractPool &operator=(const AbstractPool &) = delete;

private:
    // std::map never moves its elements, so the addresses we hand out are stable.
    std::map<AbstractNode, AbstractNode> _nodes;
    std::map<AbstractField, AbstractField> _fields;

    // (base, field) -> id of the LoadedObject node minted for it
    std::map<LoadOrigin, int> _loadedObjectIdByOrigin;
    int _nextLoadedObjectId;

    std::map<AbstractNode, LoadOrigin> _loadOrigins;
};

#endif // ABSTRACT_POOL_HPP