#ifndef TASK_H
#define TASK_H

#include <tuple>

//@PURPOSE: Provide Tasks and Results classes for equation-solver app
//
//@CLASSES:
//  Task: base class for all of the tasks
//  LinearTask: linear task, holds bx+c=y equation parameters
//  QuadraticTask: quadratic task, holds ax^2+bx+c=y equation parameters
//  TaskResults: base class for all equation results
//  LinearTaskResults: single root result
//  QuadraticTaskResults: two roots result and extremum value
//
//@DESCRIPTION: Tasks classes basicly holds only equation parameters.
// Results classes are able to hold root values with extremum where
// possible.

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
        // std::tie allows us to perform lexicographical comparison
        // to a struct or to unpack a tuple
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
