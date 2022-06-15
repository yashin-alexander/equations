#ifndef EQUATION_H
#define EQUATION_H

#include "task.h"

#include <cstdint>

//@PURPOSE: Provide an aquation class for equation-solver app
//
//@CLASSES:
//  Equation: Abstract equation class
//  LinearEquation: Linear equation class that is able to solve linear tasks
//  QuadraticEquation: square equation class that is able to solve square tasks
//
//@DESCRIPTION: For each type of class this code implements 'solve()' method
// that is implemented differently for different Equation classes.
//
// For LinearEquation this method will just calculate single root using
// 'bx+c=0' formula.
//
// For QuadraticEquation this method will firstly calculate discriminant using
// 'D=b^2-4ac' formula.

// Then, it will find both roots using discriminant value:
// 'x1 = (-b + sqrt(D))/2a'
// There is also different formula used for second root:
// x2 = (-b + sqrt(D)) / 2a
// x2 = ((-b + sqrt(D))(b + sqrt(D)) / 2a(b + sqrt(D))
// x2 = (-b^2 + D) / 2a(b + sqrt(D))
// x2 = (-b^2 + b^2 - 4ac) / 2a(b + sqrt(D))
// x2 = -2c / (b + sqrt(D))
// This second root form is better for cases when sqrt(D) is large and nearly
// equal to b. Because when we are subtracting nearly equal numbers with
// x2 = (-b + sqrt(D)) / 2a
// this leads to loss of precision.
// Suggested form is better as no such substraction needed.

class Equation
{
public:
    virtual ~Equation() = default;
    virtual void solve() = 0;
    virtual const TaskResults &get_task_results() const = 0;
    virtual bool is_solved() const { return m_solved; };

protected:
    bool m_solved {false};
};

class LinearEquation final : public Equation
{
public:
    explicit LinearEquation(const LinearTask &task) : m_task(task), m_results({}) {};
    void solve() override;
    const LinearTaskResults &get_task_results() const override;

private:
    const LinearTask &m_task;
    LinearTaskResults m_results;
};

class QuadraticEquation final : public Equation
{
public:
    explicit QuadraticEquation(const QuadraticTask &task) : m_task(task), m_results({}) {};
    void solve() override;
    const QuadraticTaskResults &get_task_results() const override;

private:
    const QuadraticTask &m_task;
    QuadraticTaskResults m_results;
    int64_t m_discriminant {};
    void calculate_discriminant();
    void calculate_roots();
    void calculate_extremum();
};

#endif // EQUATION_H
