#ifndef TASK_H
#define TASK_H

#include <optional>

struct Task {
    ~Task() = default;
};

struct LinearTask : public Task {
    int b{};
    int c{};
};

struct QuadraticTask : public LinearTask {
    int a{};
};

struct TaskResults {
    ~TaskResults() = default;
};

struct LinearTaskResults : TaskResults {
    std::optional<double> root1{std::nullopt};
};

struct QuadraticTaskResults : TaskResults {
    std::optional<double> root1{std::nullopt};
    std::optional<double> root2{std::nullopt};
    double extremum{};
};

#endif // TASK_H
