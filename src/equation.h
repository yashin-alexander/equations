#ifndef EQUATION_H
#define EQUATION_H

#include "task.h"

#include <cstdint>
#include <optional>

class Equation {
public:
    virtual ~Equation() = default;
    virtual void solve() = 0;
    virtual const TaskResults& get_task_results() const = 0;
    virtual bool is_solved() const { return m_solved; };
protected:
    bool m_solved{false};
};

class LinearEquation : public Equation {
public:
    explicit LinearEquation(const LinearTask& task) : m_task(task), m_results() {};
    void solve() override;
    const LinearTaskResults& get_task_results() const override;
private:
    const LinearTask& m_task;
    LinearTaskResults m_results;
};

class QuadraticEquation : public Equation {
public:
    explicit QuadraticEquation(const QuadraticTask& task) : m_task(task), m_results() {};
    void solve() override;
    const QuadraticTaskResults& get_task_results() const override;
private:
    const QuadraticTask& m_task;
    QuadraticTaskResults m_results;
    int64_t m_discriminant{};
    void calculate_discriminant();
    void calculate_roots();
    void calculate_extremum();
};

#endif // EQUATION_H
