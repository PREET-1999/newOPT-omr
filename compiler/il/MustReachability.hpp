#ifndef MUST_REACHABILITY_HPP
#define MUST_REACHABILITY_HPP

#include <map>
#include <set>
#include <vector>
#include <utility>
#include "il/PTG.hpp"

enum class MustReachResult {
    MUST_REACHABLE,
    NOT_REACHABLE,
    NOT_APPLICABLE
};

class MustGraph {
public:
    void addEdge(TR::Node *base, TR::SymbolReference *field, TR::Node *pointee);

std::vector<std::pair<TR::SymbolReference*, TR::Node*>>
    edgesFrom(TR::Node *base) const;

    bool hasNode(TR::Node *base) const;

    static MustGraph build(const PTG &state);

private:
    std::map<TR::Node*, std::vector<std::pair<TR::SymbolReference*, TR::Node*>>> _mustGraph;
};

bool mustReachableInGraph(const MustGraph &g, TR::Node *from, TR::Node *to);

MustReachResult mustReachableQuery(const PTG &state,
                                    const std::set<TR::Node*> &fromNodes,
                                    const std::set<TR::Node*> &toNodes);

#endif