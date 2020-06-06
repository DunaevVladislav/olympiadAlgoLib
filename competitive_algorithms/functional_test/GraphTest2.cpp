#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/Graph.h"
#include "../competitiveProgAlgoLib/Edge.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    cpa::Graph<cpa::Edge> graph(5, false);
    graph.addEdge(cpa::Edge(0, 1));
    graph.addEdge(cpa::Edge(1, 2));
    graph.addEdge(cpa::Edge(2, 0));
    graph.addEdge(cpa::Edge(2, 4));
    graph.addEdge(cpa::Edge(4, 3));
    graph.addEdge(cpa::Edge(3, 2));
    std::cout << "Граф успешно построен" << std::endl;
    std::cout << "Найденные точки сочленения:" << std::endl;
    for(auto v : graph.cutVertexes()){
        std::cout << v + 1 << std::endl;
    }
    return 0;
}