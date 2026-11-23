#include "AbstractPool.hpp"

AbstractNode *AbstractPool::getNode(AbstractNode::Kind kind, int ownerMethodId, int id) {
    AbstractNode key(kind, ownerMethodId, id);
    std::map<AbstractNode, AbstractNode>::iterator it = _nodes.find(key);
    if (it == _nodes.end())
        it = _nodes.insert(std::make_pair(key, key)).first;
    return &it->second;
}

AbstractField *AbstractPool::getField(int fieldId) {
    AbstractField key(fieldId);
    std::map<AbstractField, AbstractField>::iterator it = _fields.find(key);
    if (it == _fields.end())
        it = _fields.insert(std::make_pair(key, key)).first;
    return &it->second;
}

AbstractNode *AbstractPool::getLoadedObject(AbstractNode *base, AbstractField *field,
                                            int ownerMethodId) {
    LoadOrigin origin(*base, *field);

    std::map<LoadOrigin, int>::iterator it = _loadedObjectIdByOrigin.find(origin);
    if (it == _loadedObjectIdByOrigin.end())
        it = _loadedObjectIdByOrigin.insert(std::make_pair(origin, _nextLoadedObjectId++)).first;

    AbstractNode *node = getNode(AbstractNode::LoadedObject, ownerMethodId, it->second);
    _loadOrigins.insert(std::make_pair(*node, origin)); // no-op if already recorded
    return node;
}