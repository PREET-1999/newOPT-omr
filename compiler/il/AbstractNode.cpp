#include "AbstractNode.hpp"

#include <sstream>
#include <tuple>

bool AbstractNode::operator<(const AbstractNode &other) const {
    return std::tie(kind, id) < std::tie(other.kind, other.id);
}

bool AbstractNode::operator==(const AbstractNode &other) const {
    return kind == other.kind && id == other.id;
}

std::string AbstractNode::toString() const {
    std::ostringstream s;
    switch (kind) {
        case ParameterObject: s << "ParamObj(" << id << ")";     break;
        case Allocation:      s << "Alloc(bci=" << id << ")";    break;
        case Static:          s << "Static";                     break;
        case LoadedObject:    s << "Loaded(" << id << ")";    break;
        case Unknown:         s << "Unknown";                    break;
    }
    return s.str();
}