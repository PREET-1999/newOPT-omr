#include "AbstractNode.hpp"

#include <sstream>
#include <tuple>

bool AbstractNode::isInternal() const {
    return kind == Allocation;
}

bool AbstractNode::isExternal() const {
    return !isInternal();
}

bool AbstractNode::operator<(const AbstractNode &other) const {
    return std::tie(kind, ownerMethodId, id) <
           std::tie(other.kind, other.ownerMethodId, other.id);
}

bool AbstractNode::operator==(const AbstractNode &other) const {
    return kind == other.kind && ownerMethodId == other.ownerMethodId && id == other.id;
}

std::string AbstractNode::toString() const {
    std::ostringstream s;
    switch (kind) {
        case ParameterObject: s << "ParamObj(m" << ownerMethodId << ", " << id << ")";       break;
        case Allocation:      s << "Alloc(m" << ownerMethodId << ", bci=" << id << ")";      break;
        case LoadedObject:    s << "Loaded(m" << ownerMethodId << ", " << id << ")";         break;
        case Static:          s << "Static";                                                 break;
        case Unknown:         s << "Unknown";                                                break;
    }
    return s.str();
}