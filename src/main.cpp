#include "argparse.h"
#include "equation.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

//https://www.codeproject.com/Articles/25294/Avoiding-Overflow-Underflow-and-Loss-of-Precision

int main(int argc, char **argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::vector<QuadraticTask> tasks{};
    ArgumentParser parser{arguments};
    if (!parser.parse(tasks)) {
        return -1;
    }

    for (const auto& t: tasks) {
        std::cout << "solving: " << t.a << "x^2 + " << t.b << "x + " << t.c << "\n";
        QuadraticEquation equation{t};
        equation.solve();
        if (equation.is_solved()){
            const QuadraticTaskResults results = equation.get_task_results();
            std::cout << "\tx1 = " << results.root1.value_or(std::nanf("0")) << '\n';
            std::cout << "\tx2 = " << results.root2.value_or(std::nanf("0")) << '\n';
            std::cout << "\textremum = " << results.extremum << '\n';
        }
    }
    return 0;
}
