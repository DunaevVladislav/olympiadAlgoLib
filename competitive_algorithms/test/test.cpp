#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "./../competitiveProgAlgoLib/ISegmentSolver.h"
#include "./../competitiveProgAlgoLib/Array.h"

void test1() {
    const std::size_t count_iter = 100;
    const std::size_t max_array_size = 30;
    const std::size_t max_query_count = 100;
    std::mt19937 rnd(0);
    std::function<int(int, int)> minimum = [](const int &a, const int &b) {
        if (a < b) {
            return a;
        }
        return b;
    };

    std::cerr << "Test 1: Queries on a segment in a random array" << std::endl;
    for (std::size_t test_number = 1; test_number <= count_iter; ++test_number) {
        std::size_t array_size = rnd() % max_array_size + 1;
        std::vector<int> array(array_size);
        for (auto &val : array) {
            val = rnd();
        }
        auto query_count = std::min(array_size * (array_size + 1) / 2, max_query_count);
        std::vector<std::pair<std::size_t, std::size_t>> query(query_count);
        for (auto&[l, r] : query) {
            l = rnd() % array_size;
            r = rnd() % array_size;
            if (l > r) {
                std::swap(l, r);
            }
        }
        std::vector<std::pair<std::string, cpa::ISegmentSolver<int> *>> testing_struct{
                {"Array", new cpa::Array<int>(array.begin(), array.end(), minimum)},
        };
        for (const auto&[l, r] : query) {
            auto real_result = *std::min_element(array.begin() + l, array.begin() + r + 1);
            for (const auto&[name, structure] : testing_struct) {
                auto result = structure->valueOnSegment(l, r);
                if (result != real_result) {
                    std::cerr << "ERROR:" << std::endl;
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
    }
    std::cerr << "OK\n" << std::endl;
}

int main() {
    test1();
    return 0;
}
