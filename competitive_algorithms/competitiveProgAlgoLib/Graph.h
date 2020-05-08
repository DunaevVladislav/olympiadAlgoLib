#ifndef COMPETITIVE_ALGORITHMS_GRAPH_H
#define COMPETITIVE_ALGORITHMS_GRAPH_H

#include <cstdlib>
#include <vector>
#include <queue>
#include <set>
#include <numeric>
#include "IEdge.h"

namespace cpa {

    template<class TEdge, class TWeight = int>
    class Graph {
    private:
        typedef std::vector<std::int8_t> mark_list;
        std::vector<TEdge> m_edges;
        std::vector<std::vector<std::size_t>> m_incidentEdges;
        bool m_isDirected = false;

        void topologicalSortDfs(std::size_t t_from, mark_list &t_was, std::vector<std::size_t> &t_order) const;

        void findBridge(std::size_t t_from, int t_parentEdge, mark_list &was, std::vector<std::size_t> &t_tin,
                        std::vector<std::size_t> &t_fup, std::size_t &timer,
                        mark_list &t_is_bridge) const;

        void
        findCutVertexes(std::size_t t_from, int t_parentEdge, mark_list &was, std::vector<std::size_t> &t_tin,
                        std::vector<std::size_t> &t_fup, std::size_t &timer,
                        Graph::mark_list &isCutVertex) const;

        bool checkCycleDfs(std::size_t t_from, int t_parentEdge, mark_list &t_colors) const;

    public:
        explicit Graph(std::size_t t_vertexCount = 0, bool t_isDirected = false);

        Graph(const Graph<TEdge, TWeight> &other);

        Graph(Graph<TEdge, TWeight> &&other) noexcept;

        Graph<TEdge, TWeight> transposedGraph() const noexcept;

        [[nodiscard]] std::size_t vertexCount() const noexcept;

        [[nodiscard]] std::size_t edgeCount() const noexcept;

        std::vector<TEdge> edges() const;

        [[nodiscard]] bool isDirected() const noexcept;

        void addEdge(const TEdge &t_edge);

        [[nodiscard]] std::vector<std::vector<std::size_t>> connectivityComponents() const;

        [[nodiscard]] std::vector<std::size_t> topologicalSort() const;

        [[nodiscard]] bool isAcyclic() const;

        [[nodiscard]] std::vector<std::vector<std::size_t>> strongConnectivityComponents() const;

        [[nodiscard]] std::vector<TEdge> bridges() const;

        [[nodiscard]] std::vector<std::size_t> cutVertexes() const;

        std::pair<std::vector<TWeight>, std::vector<std::size_t>> Dijkstra(std::size_t t_from) const;

        std::pair<std::vector<TWeight>, std::vector<std::size_t>>
        FordBellman(std::size_t t_from, const TWeight &t_minusInf) const;

    };

    template<class TEdge, class TWeight>
    Graph<TEdge, TWeight>::Graph(std::size_t t_vertexCount, bool t_isDirected) {
        this->m_incidentEdges.assign(t_vertexCount, {});
        this->m_isDirected = t_isDirected;
    }

    template<class TEdge, class TWeight>
    Graph<TEdge, TWeight>::Graph(const Graph<TEdge, TWeight> &other) {
        this->m_edges = other.m_edges;
        this->m_incidentEdges = other.m_incidentEdges;
    }

    template<class TEdge, class TWeight>
    Graph<TEdge, TWeight>::Graph(Graph<TEdge, TWeight> &&other) noexcept {
        this->m_edges = std::move(other.m_edges);
        this->m_incidentEdges = std::move(other.m_incidentEdges);
    }

    template<class TEdge, class TWeight>
    Graph<TEdge, TWeight> Graph<TEdge, TWeight>::transposedGraph() const noexcept {
        Graph<TEdge, TWeight> transposedGraph(*this);
        transposedGraph.m_incidentEdges.assign(transposedGraph.vertexCount(), {});
        for (auto i = 0u; i < transposedGraph.m_edges.size(); ++i) {
            auto &e = transposedGraph.m_edges[i];
            e.reverse();
            transposedGraph.m_incidentEdges[e.from()].push_back(i);
        }
        return transposedGraph;
    }

    template<class TEdge, class TWeight>
    std::size_t Graph<TEdge, TWeight>::vertexCount() const noexcept {
        return m_incidentEdges.size();
    }

    template<class TEdge, class TWeight>
    std::size_t Graph<TEdge, TWeight>::edgeCount() const noexcept {
        if (m_isDirected) {
            return m_edges.size();
        }
        return m_edges.size() / 2;
    }

    template<class TEdge, class TWeight>
    bool Graph<TEdge, TWeight>::isDirected() const noexcept {
        return this->m_isDirected;
    }

    template<class TEdge, class TWeight>
    void Graph<TEdge, TWeight>::addEdge(const TEdge &t_edge) {
        this->m_incidentEdges[t_edge.from()].push_back(this->m_edges.size());
        this->m_edges.push_back(t_edge);
        if (!m_isDirected) {
            auto rEdge(t_edge);
            rEdge.reverse();
            this->m_incidentEdges[rEdge.from()].push_back(this->m_edges.size());
            this->m_edges.push_back(rEdge);
        }
    }

    template<class TEdge, class TWeight>
    std::vector<std::vector<std::size_t>> Graph<TEdge, TWeight>::connectivityComponents() const {
        if (this->m_isDirected) {
            Graph<TEdge, TWeight> g(this->vertexCount(), false);
            for (auto &e : this->edges()) {
                g.addEdge(e);
            }
            return g.connectivityComponents();
        }
        std::vector<std::vector<std::size_t>> comp;
        mark_list was(this->vertexCount());
        for (std::size_t startV = 0; startV < was.size(); ++startV) {
            if (!was[startV]) {
                std::queue<std::size_t> q;
                q.push(startV);
                was[startV] = true;
                comp.emplace_back();
                while (!q.empty()) {
                    auto v = q.front();
                    q.pop();
                    comp.back().push_back(v);
                    for (auto &i : this->m_incidentEdges[v]) {
                        auto u = this->m_edges[i].to();
                        if (!was[u]) {
                            q.push(u);
                            was[u] = true;
                        }
                    }
                }
            }
        }
        return comp;
    }

    template<class TEdge, class TWeight>
    std::vector<std::size_t> Graph<TEdge, TWeight>::topologicalSort() const {
        std::vector<std::size_t> order;
        mark_list was(this->vertexCount());
        for (std::size_t v = 0; v < was.size(); ++v) {
            if (!was[v]) {
                topologicalSortDfs(v, was, order);
            }
        }
        std::reverse(order.begin(), order.end());
        return order;
    }

    template<class TEdge, class TWeight>
    void
    Graph<TEdge, TWeight>::topologicalSortDfs(std::size_t t_from, mark_list &t_was,
                                              std::vector<std::size_t> &t_order) const {
        t_was[t_from] = true;
        for (auto &i : this->m_incidentEdges[t_from]) {
            auto to = this->m_edges[i].to();
            if (!t_was[to]) {
                topologicalSortDfs(to, t_was, t_order);
            }
        }
        t_order.push_back(t_from);
    }

    template<class TEdge, class TWeight>
    std::vector<std::vector<std::size_t>> Graph<TEdge, TWeight>::strongConnectivityComponents() const {
        auto rGragh = this->transposedGraph();
        std::vector<std::vector<std::size_t>> comps;
        mark_list was(this->vertexCount());
        for (auto v : this->topologicalSort()) {
            if (!was[v]) {
                comps.emplace_back();
                rGragh.topologicalSortDfs(v, was, comps.back());
            }
        }
        return comps;
    }

    template<class TEdge, class TWeight>
    std::vector<TEdge> Graph<TEdge, TWeight>::bridges() const {
        mark_list was(this->vertexCount());
        std::vector<std::size_t> tin(this->vertexCount()), fup(this->vertexCount());
        mark_list is_bridge(this->edgeCount() * 2);
        std::size_t timer = 0;
        for (std::size_t v = 0; v < was.size(); ++v) {
            if (!was[v]) {
                findBridge(v, -1, was, tin, fup, timer, is_bridge);
            }
        }

        std::vector<TEdge> bridgesList;
        for (std::size_t i = 0; i < is_bridge.size(); ++i) {
            if (is_bridge[i]) {
                bridgesList.push_back(this->m_edges[i]);
            }
        }
        return bridgesList;
    }

    template<class TEdge, class TWeight>
    void Graph<TEdge, TWeight>::findBridge(std::size_t t_from, int t_parentEdge, Graph::mark_list &was,
                                           std::vector<std::size_t> &t_tin, std::vector<std::size_t> &t_fup,
                                           size_t &timer,
                                           mark_list &t_is_bridge) const {
        was[t_from] = true;
        t_tin[t_from] = t_fup[t_from] = timer++;
        for (auto &i : this->m_incidentEdges[t_from]) {
            if ((i ^ 1u) == t_parentEdge) {
                continue;
            }
            auto to = m_edges[i].to();
            if (was[to]) {
                t_fup[t_from] = std::min(t_fup[t_from], t_tin[to]);
            } else {
                findBridge(to, i, was, t_tin, t_fup, timer, t_is_bridge);
                t_fup[t_from] = std::min(t_fup[t_from], t_fup[to]);
                if (t_fup[to] > t_tin[t_from]) {
                    t_is_bridge[i / 2 * 2] = true;
                }
            }
        }
    }

    template<class TEdge, class TWeight>
    std::vector<std::size_t> Graph<TEdge, TWeight>::cutVertexes() const {
        mark_list was(this->vertexCount());
        std::vector<std::size_t> tin(this->vertexCount()), fup(this->vertexCount());
        std::size_t timer = 0;
        mark_list isCutVertex(this->vertexCount());
        for (std::size_t v = 0; v < was.size(); ++v) {
            if (!was[v]) {
                findCutVertexes(v, -1, was, tin, fup, timer, isCutVertex);
            }
        }
        std::vector<std::size_t> cutVertexesList;
        for (std::size_t v = 0; v < isCutVertex.size(); ++v) {
            if (isCutVertex[v]) {
                cutVertexesList.push_back(v);
            }
        }
        return cutVertexesList;
    }

    template<class TEdge, class TWeight>
    void Graph<TEdge, TWeight>::findCutVertexes(std::size_t t_from, int t_parentEdge, Graph::mark_list &was,
                                                std::vector<std::size_t> &t_tin, std::vector<std::size_t> &t_fup,
                                                size_t &timer, Graph::mark_list &isCutVertex) const {
        was[t_from] = true;
        t_tin[t_from] = t_fup[t_from] = timer++;
        std::size_t dfsCount = 0;
        for (auto &i : this->m_incidentEdges[t_from]) {
            if (i == t_parentEdge) {
                continue;
            }
            auto to = this->m_edges[i].to();
            if (was[to]) {
                t_fup[t_from] = std::min(t_fup[t_from], t_tin[to]);
            } else {
                dfsCount++;
                findCutVertexes(to, i, was, t_tin, t_fup, timer, isCutVertex);
                t_fup[t_from] = std::min(t_fup[t_from], t_fup[to]);
                if (t_parentEdge != -1 && t_fup[to] >= t_tin[t_from]) {
                    isCutVertex[to] = true;
                }
            }
        }
        if (t_parentEdge == -1 && dfsCount > 1) {
            isCutVertex[t_from] = true;
        }
    }

    template<class TEdge, class TWeight>
    std::pair<std::vector<TWeight>, std::vector<std::size_t>>
    Graph<TEdge, TWeight>::Dijkstra(std::size_t t_from) const {
        std::size_t n = this->vertexCount();
        std::vector<TWeight> dist(n, TWeight());
        std::vector<std::size_t> parent(n);
        std::iota(parent.begin(), parent.end(), 0);
        mark_list initialDist(n);
        std::set<std::pair<TWeight, std::size_t>> q;
        initialDist[t_from] = 1;
        q.emplace(dist[t_from], t_from);
        while (!q.empty()) {
            auto[d, v] = *q.begin();
            q.erase(q.begin());
            for (auto &i : this->m_incidentEdges[v]) {
                auto u = this->m_edges[i].to();
                auto nd = d + this->m_edges[i].getWeight();
                if (!initialDist[u]) {
                    initialDist[u] = true;
                    parent[u] = v;
                    dist[u] = nd;
                    q.emplace(nd, u);
                } else if (dist[u] > nd) {
                    q.erase({dist[u], u});
                    parent[u] = v;
                    dist[u] = nd;
                    q.emplace(nd, u);
                }
            }
        }
        return {dist, parent};
    }

    template<class TEdge, class TWeight>
    bool Graph<TEdge, TWeight>::isAcyclic() const {
        mark_list colors(this->vertexCount());
        for (std::size_t v = 0; v < colors.size(); ++v) {
            if (colors[v] == 0) {
                if (checkCycleDfs(v, -1, colors)) {
                    return false;
                }
            }
        }
        return true;
    }

    template<class TEdge, class TWeight>
    bool Graph<TEdge, TWeight>::checkCycleDfs(std::size_t t_from, int t_parentEdge, Graph::mark_list &t_colors) const {
        t_colors[t_from]++;
        for (auto &i : this->m_incidentEdges[t_from]) {
            if (!m_isDirected && ((i ^ 1) == t_parentEdge)) {
                continue;
            }
            auto u = this->m_edges[i].to();
            if (t_colors[u] == 0) {
                if (checkCycleDfs(u, i, t_colors)) {
                    return true;
                }
            } else if (t_colors[u] == 1) {
                return true;
            }
        }
        t_colors[t_from]++;
        return false;
    }

    template<class TEdge, class TWeight>
    std::pair<std::vector<TWeight>, std::vector<std::size_t>>
    Graph<TEdge, TWeight>::FordBellman(std::size_t t_from, const TWeight &t_minusInf) const {
        std::size_t n = this->vertexCount();
        std::vector<TWeight> dist(n, TWeight());
        std::vector<std::size_t> parent(n);
        std::iota(parent.begin(), parent.end(), 0);
        mark_list initialDist(n);
        initialDist[t_from] = 1;

        for (std::size_t i = 1; i < n; ++i) {
            bool condensate = false;
            for (auto &e : this->m_edges) {
                if (!initialDist[e.from()]) {
                    continue;
                }
                auto u = e.to();
                auto nd = dist[e.from()] + e.getWeight();
                if (!initialDist[u] || dist[u] > nd) {
                    dist[u] = std::max(t_minusInf, nd);
                    initialDist[u] = 1;
                    parent[u] = e.from();
                    condensate = true;
                }
            }
            if (!condensate) {
                break;
            }
        }

        mark_list was(n);
        for (auto &e : this->m_edges) {
            if (!initialDist[e.from()]) {
                continue;
            }
            auto nd = dist[e.from()] + e.getWeight();
            if (dist[e.to()] > nd) {
                dist[e.to()] = t_minusInf;
                parent[e.to()] = e.to();
                if (!was[e.to()]) {
                    std::queue<std::size_t> q;
                    q.push(e.to());
                    was[e.to()] = true;
                    while (!q.empty()) {
                        auto v = q.front();
                        q.pop();
                        for (auto &i : this->m_incidentEdges[v]) {
                            auto u = this->m_edges[i].to();
                            if (!was[u]) {
                                dist[u] = t_minusInf;
                                parent[u] = u;
                                was[u] = true;
                                q.push(u);
                            }
                        }
                    }
                }
            }
        }
        return {dist, parent};
    }

    template<class TEdge, class TWeight>
    std::vector<TEdge> Graph<TEdge, TWeight>::edges() const {
        return this->m_edges;
    }


}

#endif //COMPETITIVE_ALGORITHMS_GRAPH_H
