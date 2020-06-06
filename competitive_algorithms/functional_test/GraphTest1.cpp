#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/Graph.h"
#include "../competitiveProgAlgoLib/Edge.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    cpa::Graph<cpa::Edge> graph(4, true);
    graph.addEdge(cpa::Edge(0, 1));
    graph.addEdge(cpa::Edge(1, 3));
    graph.addEdge(cpa::Edge(0, 2));
    graph.addEdge(cpa::Edge(2, 3));
    std::cout << "Граф успешно построен" << std::endl;
    if (graph.isAcyclic()){
        std::cout << "Граф является ацикличныи" << std::endl;
    } else {
        std::cout << "Граф является цикличный" << std::endl;
    }
    return 0;
}