#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>
#include "../competitiveProgAlgoLib/ISegmentSolver.h"
#include "../competitiveProgAlgoLib/Array.h"
#include "../competitiveProgAlgoLib/ISparseTable.h"
#include "../competitiveProgAlgoLib/SegmentTree.h"
#include "../competitiveProgAlgoLib/FenwickTree.h"
#include "../competitiveProgAlgoLib/SqrtDecomposer.h"
#include "../competitiveProgAlgoLib/Treap.h"

void test1() {
    const std::size_t count_iter = 100;
    const std::size_t max_array_size = 100;
    const std::size_t max_query_count = 100;
    const std::size_t max_abs_val = 100;
    std::mt19937 rnd(0);
    std::function<int(int, int)> minimum = [](const int &a, const int &b) {
        if (a < b) {
            return a;
        }
        return b;
    };

    std::cerr << "Test group 1: Queries on a segment in a random array" << std::endl;
    for (std::size_t test_number = 1; test_number <= count_iter; ++test_number) {
        std::size_t array_size = rnd() % max_array_size + 1;
        std::vector<int> array(array_size);
        for (auto &val : array) {
            val = rnd() % (2 * max_abs_val + 1) - max_abs_val;
        }
        auto query_count = std::min(array_size * (array_size + 1) / 2, max_query_count);
        std::vector<std::pair<std::size_t, std::size_t>> query(query_count);
        for (auto&[l, r] : query) {
            l = rnd() % array.size();
            r = rnd() % array.size();
            if (l > r) {
                std::swap(l, r);
            }
        }
        std::vector<std::pair<std::string, cpa::ISegmentSolver<int> *>> testing_struct{
                {"Array",           new cpa::Array<int>(array.begin(), array.end(), minimum)},
                {"Sparse table",    new cpa::SparseTable<int>(array.begin(), array.end(), minimum)},
                {"Segment tree",    new cpa::SegmentTree<int>(array.begin(), array.end(), minimum)},
                {"Fenwick tree",    new cpa::FenwickTree<int>(array.begin(), array.end(), minimum)},
                {"Sqrt decomposer", new cpa::SqrtDecomposer<int>(array.begin(), array.end(), minimum)},
                {"Treap",           new cpa::Treap<int>(array.begin(), array.end(), minimum)},
        };
        for (const auto&[l, r] : query) {
            auto real_result = std::accumulate(array.begin() + l, array.begin() + r + 1, INT_MAX, minimum);
            for (const auto&[name, structure] : testing_struct) {
                int result{};
                bool was_exception = false;
                try {
                    result = structure->valueOnSegment(l, r);
                } catch (...) {
                    was_exception = true;
                }
                if (was_exception || result != real_result) {
                    if (was_exception) {
                        std::cerr << "EXCEPTION:" << std::endl;
                    } else {
                        std::cerr << "ERROR:" << std::endl;
                    }
                    std::cerr << "\tData: [";
                    for (const auto &val : array) {
                        std::cerr << val << ", ";
                    }
                    std::cerr << "\b\b]" << std::endl;
                    std::cerr << "\tValue on segment (" << l + 1 << "," << r + 1 << ") of " << name << ": " << result
                              << std::endl;
                    std::cerr << "\tExpected value: " << real_result << std::endl;
                    exit(1);
                }
            }
        }
        std::cerr << "\tTest 1." << test_number << ": OK" << std::endl;
    }
    std::cerr << "Test group 1 OK\n" << std::endl;
}

int main() {
    test1();
    return 0;
}
