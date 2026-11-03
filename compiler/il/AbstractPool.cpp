#include "AbstractPool.hpp"

AbstractNode *AbstractPool::getNode(AbstractNode::Kind kind, int id) {
    AbstractNode key(kind, id);
    std::map<AbstractNode, AbstractNode>::iterator it = _nodes.find(key);
    if (it == _nodes.end())
        it = _nodes.insert(std::make_pair(key, key)).first;
    return &it->second;
}

AbstractField *AbstractPool::getField(int constantPoolId) {
    AbstractField key(constantPoolId);
    std::map<AbstractField, AbstractField>::iterator it = _fields.find(key);
    if (it == _fields.end())
        it = _fields.insert(std::make_pair(key, key)).first;
    return &it->second;
}