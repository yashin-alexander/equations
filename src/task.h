#ifndef TASK_H
#define TASK_H

#include <tuple>

struct Task {};

struct LinearTask final : public Task {
    int b;
    int c;
    LinearTask() = default;
    LinearTask(int b, int c) : b(b), c(c) {};
};

struct QuadraticTask final : public Task {
    int a;
    int b;
    int c;
    QuadraticTask() = default;
    QuadraticTask(int a, int b, int c) : a(a), b(b), c(c) {};
    bool operator<(const QuadraticTask &other) const
    {
        /*
         * std::tie allows us to perform lexicographical comparison
         * to a struct or to unpack a tuple
         */
        return std::tie(a, b, c) < std::tie(other.a, other.b, other.c);
    };
};

struct TaskResults {};

struct LinearTaskResults final : TaskResults {
    double root1;
};

struct QuadraticTaskResults final : TaskResults {
    double root1;
    double root2;
    double extremum;
};

#endif // TASK_H
