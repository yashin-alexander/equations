cmake_minimum_required(VERSION 3.5)

project(equations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(equations src/main.cpp src/equation.h src/equation.cpp src/argparse.h src/argparse.cpp)
