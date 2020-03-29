cmake_minimum_required(VERSION 3.15)
project(competitive_algorithms)

set(CMAKE_CXX_STANDARD 17)

add_executable(testing test/test.cpp competitiveProgAlgoLib/ISegmentSolver.h)