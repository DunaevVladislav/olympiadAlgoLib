#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/SegmentTree.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    auto minimum = [&](const int &a, const int &b) {
        if (a < b) return a;
        return b;
    };
    std::vector data{4, 3, 0, 7, 8};
    cpa::SegmentTree<int> segmentTree(data.begin(), data.end(), minimum);
    std::cout << "Дерево отрезков успешно построено" << std::endl;
    for (int i = 0; i < segmentTree.size(); ++i) {
        for (int j = i; j < segmentTree.size(); ++j) {
            std::cout << "Минимум на отрезке [" << i + 1 << ", " << j + 1 << "] = ";
            std::cout << segmentTree.valueOnSegment(i, j) << std::endl;
        }
    }
    return 0;
}