#include "il/MustReachability.hpp"

void MustGraph::addEdge(TR::Node *base, TR::SymbolReference *field, TR::Node *pointee)
{
    _mustGraph[base].push_back({field, pointee});
}

std::vector<std::pair<TR::SymbolReference*, TR::Node*>>
MustGraph::edgesFrom(TR::Node *base) const
{
    auto it = _mustGraph.find(base);
    return (it != _mustGraph.end()) ? it->second : std::vector<std::pair<TR::SymbolReference*, TR::Node*>>{};
}

bool MustGraph::hasNode(TR::Node *base) const
{
    return _mustGraph.find(base) != _mustGraph.end();
}

MustGraph MustGraph::build(const PTG &state)
{
    MustGraph g;
    for (const auto &kv : state._heap)
    {
        TR::Node *base = kv.first.first;
        TR::SymbolReference *field = kv.first.second;
        const std::set<TR::Node*> &pointees = kv.second;

        TR::Node *uniquePointee = PTG::mustPointee(pointees);
        if (uniquePointee != nullptr)
            g.addEdge(base, field, uniquePointee);
    }
    return g;
}

bool mustReachableInGraph(const MustGraph &g, TR::Node *from, TR::Node *to)
{
    if (from == to)
        return true;

    std::set<TR::Node*> visited;
    std::vector<TR::Node*> worklist{from};

    while (!worklist.empty())
    {
        TR::Node *cur = worklist.back();
        worklist.pop_back();
        
        bool alreadyVisited = visited.count(cur) > 0;
        if (alreadyVisited)
            continue;
        visited.insert(cur);

        for (const auto &fieldEdge : g.edgesFrom(cur))
        {
            TR::Node *next = fieldEdge.second;
            if (next == to)
                return true;
            worklist.push_back(next);
        }
    }
    return false;
}

MustReachResult mustReachableQuery(const PTG &state,
                                    const std::set<TR::Node*> &fromNodes,
                                    const std::set<TR::Node*> &toNodes)
{
    TR::Node *A = PTG::mustPointee(fromNodes);
    TR::Node *B = PTG::mustPointee(toNodes);

    if (A == nullptr || B == nullptr)
        return MustReachResult::NOT_APPLICABLE;

    MustGraph g = MustGraph::build(state);
    return mustReachableInGraph(g, A, B)
               ? MustReachResult::MUST_REACHABLE
               : MustReachResult::NOT_REACHABLE;
}