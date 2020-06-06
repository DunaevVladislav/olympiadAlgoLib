#include <windows.h>
#include <iostream>
#include "../competitiveProgAlgoLib/SegmentTree.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    auto maximum = [&](const int &a, const int &b) {
        if (a > b) return a;
        return b;
    };
    std::vector data{89, 12, 17, 4, 9};
    cpa::SegmentTree<int> segmentTree(data.begin(), data.end(), maximum);
    std::cout << "Дерево отрезков успешно построено" << std::endl;
    std::cout << "Максимум на отрезке [2, 4] = " << segmentTree.valueOnSegment(1,3) << std::endl;
    return 0;
}