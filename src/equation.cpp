#include "equation.h"

#include <cmath>
#include <limits>

void LinearEquation::solve()
{
    if (!is_solved()) {
        if (m_task.b != 0) {
            // bx + c = 0
            m_results.root1 = static_cast<double>(-m_task.c) / m_task.b;
        } else {
            // c = y
            m_results.root1 = std::numeric_limits<double>::quiet_NaN();
        }
        m_solved = true;
    }
}

const LinearTaskResults &LinearEquation::get_task_results() const
{
    return m_results;
}

void QuadraticEquation::solve()
{
    if (!m_solved) {
        if (m_task.a != 0) {
            // ax^2 + bx + c = 0
            calculate_discriminant();
            calculate_roots();
            calculate_extremum();
        } else {
            // bx + c = 0
            LinearTask linear_task {m_task.b, m_task.c};
            LinearEquation linear_equation {linear_task};
            linear_equation.solve();
            m_results.root1 = linear_equation.get_task_results().root1;
            m_results.root2 = std::numeric_limits<double>::quiet_NaN();
            m_results.extremum = std::numeric_limits<double>::quiet_NaN();
        }
        m_solved = true;
    }
}

void QuadraticEquation::calculate_discriminant()
{
    m_discriminant = static_cast<int64_t>(m_task.b) * m_task.b -
                     static_cast<int64_t>(4) * m_task.a * m_task.c;
}

void QuadraticEquation::calculate_roots()
{
    if (m_discriminant > 0) {
        m_results.root1 = (-m_task.b - std::sqrt(m_discriminant)) / (2.0 * m_task.a);
        /*
         * root2 form is the equivalent of common second form:
         * x2 = (-b + sqrt(D)) / 2a
         * x2 = ((-b + sqrt(D))(b + sqrt(D)) / 2a(b + sqrt(D))
         * x2 = (-b^2 + D) / 2a(b + sqrt(D))
         * x2 = (-b^2 + b^2 - 4ac) / 2a(b + sqrt(D))
         * x2 = -2c / (b + sqrt(D))
         *
         * This form is better for cases when sqrt(D) is large and nearly equal to b.
         * because when we are subtracting nearly equal numbers with
         * x2 = (-b + sqrt(D)) / 2a
         * this leads to loss of precision.
         * Suggested form is better as no such substraction needed.
         */
        m_results.root2 = (-2.0 * m_task.c) / (m_task.b + std::sqrt(m_discriminant));
    } else if (m_discriminant == 0) {
        m_results.root1 = -static_cast<double>(m_task.b) / (2.0 * m_task.a);
        m_results.root2 = m_results.root1;
    } else {
        // No real roots
        m_results.root1 = std::numeric_limits<double>::quiet_NaN();
        m_results.root2 = m_results.root1;
    }
}

void QuadraticEquation::calculate_extremum()
{
    LinearTask linear_task {m_task.a * 2, m_task.b};
    auto linear_equation = LinearEquation {linear_task};
    linear_equation.solve();
    m_results.extremum = linear_equation.get_task_results().root1;
}

const QuadraticTaskResults &QuadraticEquation::get_task_results() const
{
    return m_results;
}
