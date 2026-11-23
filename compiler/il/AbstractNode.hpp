#ifndef ABSTRACT_NODE_HPP
#define ABSTRACT_NODE_HPP

#include <string>

// A node in the points-to graph, named by role instead of by TR::Node*.
// Shared by the working PTG (during analysis) and MethodSummary (persistent).
class AbstractNode {
public:
    enum Kind { ParameterObject, Allocation, Static, Unknown, LoadedObject };

    // Owner id used by nodes that belong to no particular method (Static, Unknown).
    static const int GLOBAL_OWNER = -1;

    Kind kind;
    int ownerMethodId; // id of the method this node belongs to. Must come from a table that is
                       // stable across compilations (e.g. stable method name -> int).
                       // GLOBAL_OWNER for Static / Unknown.
    int id;            // ParameterObject: param index (0 = this); phantom object the
                       //                  parameter points to at method entry
                       // Allocation:      bytecode index
                       // LoadedObject:    id minted per (base node, field) by the pool;
                       //                  the node that "p.f" held at method entry
                       // Static/Unknown:  0

    AbstractNode() : kind(Unknown), ownerMethodId(GLOBAL_OWNER), id(0) {}
    AbstractNode(Kind k, int owner, int i) : kind(k), ownerMethodId(owner), id(i) {}

    // Internal = created by the method's own code (Allocation).
    // External = stands for something that exists before the method runs
    //            (ParameterObject, LoadedObject, Static, Unknown).
    bool isInternal() const;
    bool isExternal() const;

    bool operator<(const AbstractNode &other) const;
    bool operator==(const AbstractNode &other) const;

    std::string toString() const;
};

#endif // ABSTRACT_NODE_HPP