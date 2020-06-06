#define CATCH_CONFIG_MAIN

#include "../catch2/catch.h"

#include "../competitiveProgAlgoLib/Graph.h"
#include "../competitiveProgAlgoLib/Edge.h"
#include "../competitiveProgAlgoLib/WeightedEdge.h"
#include "../competitiveProgAlgoLib/SuffixAutomaton.h"
#include "../competitiveProgAlgoLib/Array.h"
#include "../competitiveProgAlgoLib/SparseTable.h"
#include "../competitiveProgAlgoLib/SegmentTree.h"
#include "../competitiveProgAlgoLib/FenwickTree.h"
#include "../competitiveProgAlgoLib/SqrtDecomposer.h"
#include "../competitiveProgAlgoLib/Treap.h"


TEST_CASE("Directed graph testing", "[graph]") {

    cpa::Graph<cpa::Edge> g(5, true);
    REQUIRE(g.vertexCount() == 5);
    REQUIRE(g.isDirected());

    g.addEdge(cpa::Edge(2, 1));
    g.addEdge(cpa::Edge(1, 4));
    g.addEdge(cpa::Edge(0, 3));
    g.addEdge(cpa::Edge(3, 4));
    g.addEdge(cpa::Edge(2, 0));
    REQUIRE(g.edgeCount() == 5);

    SECTION("add edge") {
        g.addEdge(cpa::Edge(0, 4));
        REQUIRE(g.edgeCount() == 6);
        g.addEdge(cpa::Edge(0, 4));
        g.addEdge(cpa::Edge(0, 4));
        REQUIRE(g.edgeCount() == 8);
    }

    SECTION("connectivity components") {
        auto comp = g.connectivityComponents();
        REQUIRE(comp.size() == 1);
        REQUIRE_THAT(comp[0], Catch::UnorderedEquals(std::vector{0u, 1u, 2u, 3u, 4u}));
    }

    SECTION("topological sort") {
        auto order = g.topologicalSort();
        REQUIRE(order.size() == g.vertexCount());
        std::vector<std::size_t> pos(g.vertexCount());
        for (auto i = 0u; i < order.size(); ++i) {
            pos[order[i]] = i;
        }
        for (auto &e : g.edges()) {
            if (pos[e.from()] >= pos[e.to()]) {
                REQUIRE(pos[e.from()] < pos[e.to()]);
            }
        }
    }

    SECTION("is acyclic") {
        REQUIRE(g.isAcyclic());
        g.addEdge(cpa::Edge(0, 4));
        REQUIRE(g.isAcyclic());
        g.addEdge(cpa::Edge(3, 2));
        REQUIRE_FALSE(g.isAcyclic());
    }

    SECTION("strong connectivity components") {
        auto comp = g.strongConnectivityComponents();
        REQUIRE(comp.size() == 5);
        std::vector<bool> was(g.vertexCount());
        for (auto &a : comp) {
            REQUIRE(a.size() == 1);
            for (auto &v : a) {
                INFO("vertex should be unique")
                REQUIRE(!was[v]);
                was[v] = true;
            }
        }

        g.addEdge(cpa::Edge(0, 4));
        comp = g.strongConnectivityComponents();
        REQUIRE(comp.size() == 5);
        was.assign(g.vertexCount(), false);
        for (auto &a : comp) {
            REQUIRE(a.size() == 1);
            for (auto &v : a) {
                INFO("vertex should be unique")
                REQUIRE(!was[v]);
                was[v] = true;
            }
        }

        g.addEdge(cpa::Edge(3, 2));
        comp = g.strongConnectivityComponents();
        REQUIRE(comp.size() == 3);
        was.assign(g.vertexCount(), false);
        for (auto &a : comp) {
            REQUIRE((a.size() == 1 || a.size() == 3));
            for (auto &v : a) {
                INFO("vertex should be unique")
                REQUIRE(!was[v]);
                was[v] = true;
            }
            if (a.size() == 3) {
                REQUIRE_THAT(a, Catch::UnorderedEquals(std::vector{0u, 2u, 3u}));
            }
        }

        g.addEdge(cpa::Edge(4, 2));
        comp = g.strongConnectivityComponents();
        REQUIRE(comp.size() == 1);
        was.assign(g.vertexCount(), false);
        for (auto &a : comp) {
            REQUIRE(a.size() == 5);
            for (auto &v : a) {
                INFO("vertex should be unique")
                REQUIRE(!was[v]);
                was[v] = true;
            }
            REQUIRE_THAT(a, Catch::UnorderedEquals(std::vector{0u, 1u, 2u, 3u, 4u}));
        }
    }

    SECTION("Dijkstra") {
        auto[dist, p] = g.Dijkstra(2);
        REQUIRE_THAT(dist, Catch::Equals(std::vector{1, 1, 0, 2, 2}));
        REQUIRE_THAT(p, Catch::Equals(std::vector{2u, 2u, 2u, 0u, 1u}));
    }

    SECTION("Ford-Bellman") {
        auto[dist, p] = g.FordBellman(2, -1);
        REQUIRE_THAT(dist, Catch::Equals(std::vector{1, 1, 0, 2, 2}));
        REQUIRE_THAT(p, Catch::Equals(std::vector{2u, 2u, 2u, 0u, 1u}));
    }

}

TEST_CASE("Undirected graph testing", "[graph]") {

    cpa::Graph<cpa::Edge, int> g(6, false);
    REQUIRE(g.vertexCount() == 6);
    REQUIRE(g.edgeCount() == 0);

    SECTION("connectivity components") {
        auto comp = g.connectivityComponents();
        REQUIRE(comp.size() == 6);

        g.addEdge(cpa::Edge(3, 2));
        g.addEdge(cpa::Edge(2, 4));
        g.addEdge(cpa::Edge(3, 4));
        comp = g.connectivityComponents();
        REQUIRE(comp.size() == 4);

        g.addEdge(cpa::Edge(0, 1));
        g.addEdge(cpa::Edge(1, 5));
        comp = g.connectivityComponents();
        REQUIRE(comp.size() == 2);

        g.addEdge(cpa::Edge(1, 3));
        comp = g.connectivityComponents();
        REQUIRE(comp.size() == 1);
        REQUIRE_THAT(comp[0], Catch::UnorderedEquals(std::vector{0u, 1u, 2u, 3u, 4u, 5u}));
    }

    g.addEdge(cpa::Edge(3, 2));
    g.addEdge(cpa::Edge(3, 4));
    g.addEdge(cpa::Edge(0, 1));
    g.addEdge(cpa::Edge(1, 5));

    SECTION("is acyclic") {
        REQUIRE(g.isAcyclic());

        g.addEdge(cpa::Edge(2, 4));
        REQUIRE_FALSE(g.isAcyclic());
    }

    SECTION("BRIDGES") {
        auto bridges = g.bridges();
        REQUIRE(bridges.size() == 4);

        g.addEdge(cpa::Edge(2, 4));
        bridges = g.bridges();
        REQUIRE(bridges.size() == 2);

        g.addEdge(cpa::Edge(5, 0));
        bridges = g.bridges();
        REQUIRE(bridges.empty());

        g.addEdge(cpa::Edge(1, 3));
        bridges = g.bridges();
        REQUIRE(bridges.size() == 1);
    }

    SECTION("CUT VERTEX") {
        auto cutVertexes = g.cutVertexes();
        REQUIRE(cutVertexes.size() == 2);

        g.addEdge(cpa::Edge(2, 4));
        cutVertexes = g.cutVertexes();
        REQUIRE(cutVertexes.size() == 1);

        g.addEdge(cpa::Edge(5, 0));
        cutVertexes = g.cutVertexes();
        REQUIRE(cutVertexes.empty());

        g.addEdge(cpa::Edge(1, 3));
        cutVertexes = g.cutVertexes();
        REQUIRE(cutVertexes.size() == 2);
    }

}

TEST_CASE("Shortest path in graph", "[graph]") {

    SECTION("bamboo graph") {
        cpa::Graph<cpa::WeightedEdge<int>, int> g(10);
        g.addEdge(cpa::WeightedEdge<int>(0, 1, 1));
        g.addEdge(cpa::WeightedEdge<int>(1, 2, 2));
        g.addEdge(cpa::WeightedEdge<int>(2, 3, 3));
        g.addEdge(cpa::WeightedEdge<int>(3, 4, 4));
        g.addEdge(cpa::WeightedEdge<int>(4, 5, 5));
        g.addEdge(cpa::WeightedEdge<int>(5, 6, 6));
        g.addEdge(cpa::WeightedEdge<int>(6, 7, 7));
        g.addEdge(cpa::WeightedEdge<int>(7, 8, 8));
        g.addEdge(cpa::WeightedEdge<int>(8, 9, 9));

        auto[dist1, p1] = g.Dijkstra(0);
        REQUIRE_THAT(dist1, Catch::Equals(std::vector{0, 1, 3, 6, 10, 15, 21, 28, 36, 45}));
        REQUIRE_THAT(p1, Catch::Equals(std::vector<unsigned>{0, 0, 1, 2, 3, 4, 5, 6, 7, 8}));

        auto[dist2, p2] = g.FordBellman(0, -1);
        REQUIRE_THAT(dist2, Catch::Equals(std::vector{0, 1, 3, 6, 10, 15, 21, 28, 36, 45}));
        REQUIRE_THAT(p2, Catch::Equals(std::vector<unsigned>{0, 0, 1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("star graph") {
        cpa::Graph<cpa::WeightedEdge<int>, int> g(10);
        g.addEdge(cpa::WeightedEdge<int>(0, 1, 1));
        g.addEdge(cpa::WeightedEdge<int>(0, 2, 2));
        g.addEdge(cpa::WeightedEdge<int>(0, 3, 3));
        g.addEdge(cpa::WeightedEdge<int>(0, 4, 4));
        g.addEdge(cpa::WeightedEdge<int>(0, 5, 5));
        g.addEdge(cpa::WeightedEdge<int>(0, 6, 6));
        g.addEdge(cpa::WeightedEdge<int>(0, 7, 7));
        g.addEdge(cpa::WeightedEdge<int>(0, 8, 8));
        g.addEdge(cpa::WeightedEdge<int>(0, 9, 9));

        auto[dist1, p1] = g.Dijkstra(0);
        REQUIRE_THAT(dist1, Catch::Equals(std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE_THAT(p1, Catch::Equals(std::vector<unsigned>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

        auto[dist2, p2] = g.FordBellman(0, -1);
        REQUIRE_THAT(dist2, Catch::Equals(std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE_THAT(p2, Catch::Equals(std::vector<unsigned>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
    }

    SECTION("directed graph") {
        cpa::Graph<cpa::WeightedEdge<int>, int> g(10, true);
        g.addEdge(cpa::WeightedEdge(0, 2, 4));
        g.addEdge(cpa::WeightedEdge(2, 3, 0));
        g.addEdge(cpa::WeightedEdge(3, 0, 2));
        g.addEdge(cpa::WeightedEdge(2, 7, 15));
        g.addEdge(cpa::WeightedEdge(3, 7, 11));
        g.addEdge(cpa::WeightedEdge(0, 1, 3));
        g.addEdge(cpa::WeightedEdge(0, 9, 12));
        g.addEdge(cpa::WeightedEdge(7, 1, 6));
        g.addEdge(cpa::WeightedEdge(7, 4, 3));
        g.addEdge(cpa::WeightedEdge(1, 5, 100));
        g.addEdge(cpa::WeightedEdge(9, 8, 13));
        g.addEdge(cpa::WeightedEdge(5, 8, 4));
        g.addEdge(cpa::WeightedEdge(8, 6, 0));
        g.addEdge(cpa::WeightedEdge(6, 0, 7));
        g.addEdge(cpa::WeightedEdge(8, 4, 17));

        SECTION("start from 1") {
            auto[dist1, p1] = g.Dijkstra(1);
            REQUIRE_THAT(dist1, Catch::Equals(std::vector{111, 0, 115, 115, 121, 100, 104, 126, 104, 123}));
            REQUIRE_THAT(p1, Catch::Equals(std::vector<unsigned>{6, 1, 0, 2, 8, 1, 8, 3, 5, 0}));

            auto[dist2, p2] = g.FordBellman(1, -1);
            REQUIRE_THAT(dist2, Catch::Equals(std::vector{111, 0, 115, 115, 121, 100, 104, 126, 104, 123}));
            REQUIRE_THAT(p2, Catch::Equals(std::vector<unsigned>{6, 1, 0, 2, 8, 1, 8, 3, 5, 0}));
        }

        SECTION("start from 4") {
            auto[dist1, p1] = g.Dijkstra(4);
            REQUIRE_THAT(dist1, Catch::Equals(std::vector{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
            REQUIRE_THAT(p1, Catch::Equals(std::vector<unsigned>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

            auto[dist2, p2] = g.FordBellman(4, -1);
            REQUIRE_THAT(dist2, Catch::Equals(std::vector{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
            REQUIRE_THAT(p2, Catch::Equals(std::vector<unsigned>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        }

    }

    SECTION("undirected graph") {
        cpa::Graph<cpa::WeightedEdge<int>, int> g(6, false);
        g.addEdge(cpa::WeightedEdge(1, 3, 8));
        g.addEdge(cpa::WeightedEdge(1, 4, 9));
        g.addEdge(cpa::WeightedEdge(4, 3, 1));
        g.addEdge(cpa::WeightedEdge(1, 2, 12));
        g.addEdge(cpa::WeightedEdge(4, 2, 0));
        g.addEdge(cpa::WeightedEdge(4, 5, 25));
        g.addEdge(cpa::WeightedEdge(5, 3, 13));
        g.addEdge(cpa::WeightedEdge(2, 5, 4));

        auto[dist1, p1] = g.Dijkstra(3);
        REQUIRE_THAT(dist1, Catch::Equals(std::vector{0, 8, 1, 0, 1, 5}));
        REQUIRE_THAT(p1, Catch::Equals(std::vector<unsigned>{0, 3, 4, 3, 3, 2}));

        auto[dist2, p2] = g.FordBellman(3, -1);
        REQUIRE_THAT(dist2, Catch::Equals(std::vector{0, 8, 1, 0, 1, 5}));
        REQUIRE_THAT(p2, Catch::Equals(std::vector<unsigned>{0, 3, 4, 3, 3, 2}));
    }

    SECTION("graph with negative weight") {
        cpa::Graph<cpa::WeightedEdge<int>, int> g(9, true);
        g.addEdge(cpa::WeightedEdge<int>(0, 4, 1));
        g.addEdge(cpa::WeightedEdge<int>(5, 4, -9));
        g.addEdge(cpa::WeightedEdge<int>(4, 0, 2));
        g.addEdge(cpa::WeightedEdge<int>(4, 8, -3));
        g.addEdge(cpa::WeightedEdge<int>(8, 0, 2));
        g.addEdge(cpa::WeightedEdge<int>(0, 1, 4));
        g.addEdge(cpa::WeightedEdge<int>(1, 7, 3));
        g.addEdge(cpa::WeightedEdge<int>(7, 2, 1));
        g.addEdge(cpa::WeightedEdge<int>(2, 1, -5));
        g.addEdge(cpa::WeightedEdge<int>(7, 3, 2));
        g.addEdge(cpa::WeightedEdge<int>(3, 6, 7));
        g.addEdge(cpa::WeightedEdge<int>(6, 7, 6));

        const int NEG_INF = -10000;
        auto[dist, p] = g.FordBellman(4, NEG_INF);
        REQUIRE_THAT(dist, Catch::Equals(std::vector{-1, NEG_INF, NEG_INF, NEG_INF, 0, 0, NEG_INF, NEG_INF, -3}));
        REQUIRE_THAT(p, Catch::Equals(std::vector<unsigned>{8, 1, 2, 3, 4, 5, 6, 7, 4}));
    }

}

TEST_CASE("Suffix automaton", "[suf_automaton]") {

    std::string s = "qwerty_q";
    cpa::SuffixAutomaton a(s);

    INFO("find all characters besides last")
    for (std::size_t i = 0; i + 1 < s.length(); ++i) {
        REQUIRE(a.find(std::string(1, s[i])) == i);
    }

    REQUIRE(a.find("qwe") == 0);
    REQUIRE(a.find("wer") == 1);
    REQUIRE(a.find(s) == 0);
    REQUIRE(a.find("_q") == 6);
    REQUIRE(a.find("") == 0);

    REQUIRE(a.find("z") == -1);
    REQUIRE(a.find(s + "asd") == -1);
    REQUIRE(a.find("qwerty_w") == -1);
    REQUIRE(a.find("erty2") == -1);

    auto pos_q = a.findAll("q");
    REQUIRE_THAT(pos_q, Catch::UnorderedEquals(std::vector{0, 7}));
    auto pos_empty = a.findAll("");
    REQUIRE_THAT(pos_empty, Catch::UnorderedEquals(std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8}));

    SECTION("copy constructor") {
        auto a2 = a;
        REQUIRE(a2.find("qwe") == 0);
        REQUIRE(a2.find(s) == 0);
        REQUIRE(a2.find("_q") == 6);
        REQUIRE(a2.find(s + "asd") == -1);
        REQUIRE(a2.find("erty2") == -1);
    }

    SECTION("long string of equals character") {
        std::string sa(1000, 'a');
        cpa::SuffixAutomaton a2(sa);
        REQUIRE(a2.find("a") == 0);
        std::vector<int> pos(sa.length());
        std::iota(pos.begin(), pos.end(), 0);
        REQUIRE_THAT(a2.findAll("a"), Catch::UnorderedEquals(pos));
    }

}

TEST_CASE("Range query", "[data_structure]") {

    auto minimum = [](const int &a, const int &b) {
        if (a < b) {
            return a;
        }
        return b;
    };

    SECTION("handle tests") {
        std::vector data{12, 4, 6, -5, 2, 90};

        SECTION("array") {
            cpa::Array<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);

            ds.changeValueAt(3, 7);
            REQUIRE(ds.getValueAt(3) == 7);
            REQUIRE(ds.valueOnSegment(0, 5) == 2);
        }

        SECTION("sparse table") {
            cpa::SparseTable<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);
        }

        SECTION("segment tree") {
            cpa::SegmentTree<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);

            ds.changeValueAt(3, 7);
            REQUIRE(ds.getValueAt(3) == 7);
            REQUIRE(ds.valueOnSegment(0, 5) == 2);
        }

        SECTION("fenwick tree") {
            cpa::FenwickTree<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);

            ds.changeValueAt(5, -80);
            REQUIRE(ds.getValueAt(5) == -80);
            REQUIRE(ds.valueOnSegment(0, 5) == -80);
        }

        SECTION("sqrt decomposition") {
            cpa::SqrtDecomposer<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);

            ds.changeValueAt(3, 7);
            REQUIRE(ds.getValueAt(3) == 7);
            REQUIRE(ds.valueOnSegment(0, 5) == 2);
        }

        SECTION("treap") {
            cpa::Treap<int> ds(data.begin(), data.end(), minimum);
            for (std::size_t i = 0; i < data.size(); ++i) {
                REQUIRE(ds.valueOnSegment(i, i) == data[i]);
                REQUIRE(ds.getValueAt(i) == data[i]);
            }
            REQUIRE(ds.valueOnSegment(0, 5) == -5);
            REQUIRE(ds.valueOnSegment(0, 2) == 4);
            REQUIRE(ds.valueOnSegment(4, 5) == 2);

            ds.changeValueAt(3, 7);
            REQUIRE(ds.getValueAt(3) == 7);
            REQUIRE(ds.valueOnSegment(0, 5) == 2);
        }
    }

    SECTION("random static tests") {
        std::vector<int> data(GENERATE(take(100, random(1, 100))));
        for (auto &val : data) {
            val = GENERATE(take(1, random(-1000, 1000)));
        }
        std::vector<cpa::ISegmentSolver<int> *> testing_struct{
                new cpa::Array<int>(data.begin(), data.end(), minimum),
                new cpa::SparseTable<int>(data.begin(), data.end(), minimum),
                new cpa::SegmentTree<int>(data.begin(), data.end(), minimum),
                new cpa::FenwickTree<int>(data.begin(), data.end(), minimum),
                new cpa::SqrtDecomposer<int>(data.begin(), data.end(), minimum),
                new cpa::Treap<int>(data.begin(), data.end(), minimum),
        };
        for (auto &ds : testing_struct) {
            for (std::size_t l = 0; l < data.size(); ++l) {
                auto mn = data[l];
                for (std::size_t r = l; r < data.size(); ++r) {
                    mn = minimum(mn, data[r]);
                    REQUIRE(ds->valueOnSegment(l, r) == mn);
                }
            }
            delete ds;
        }
    }

}