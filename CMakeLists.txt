cmake_minimum_required(VERSION 3.5)

project(equations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(equations
    src/main.cpp
    src/equation.h
    src/equation.cpp
    src/argparse.h
    src/argparse.cpp
    src/ipc.h
    src/producer.h
    src/consumer.h)
target_link_libraries(equations -lpthread) # For some reason #include <semaphore> cannot link due to absence of pthread
