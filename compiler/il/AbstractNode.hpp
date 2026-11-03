#ifndef ABSTRACT_NODE_HPP
#define ABSTRACT_NODE_HPP

#include <string>

// A node in the points-to graph, named by role instead of by TR::Node*.
// Shared by the working PTG (during analysis) and MethodSummary (persistent).
class AbstractNode {
public:
    enum Kind { ParameterObject, Allocation, Static, Unknown, LoadedObject};
    enum Origin{ Internal, External };

    
    Kind kind;
    int id; // ParameterObject: param index (0 = this); phantom object the
            //                  parameter points to at method entry
            // Allocation:      bytecode index
    Origin origin;

    AbstractNode(Kind k = Unknown, int i = 0) : kind(k), origin(k == Allocation ? Internal : External), id(i) {}

     bool isExternal() const { return origin == External; }
    bool isInternal() const { return origin == Internal; }

    bool operator<(const AbstractNode &other) const;
    bool operator==(const AbstractNode &other) const;

    std::string toString() const;
};

#endif // ABSTRACT_NODE_HPP