#include "equation.h"

#include <iostream>
#include <cmath>

void LinearEquation::solve() {
    if (!is_solved()){
        if (m_task.b != 0) {
            // bx + c = 0
            m_results.root1 = static_cast<double>(-m_task.c) / m_task.b;
        } else {
            // c = y
            m_results.root1.reset();
        }
        m_solved = true;
    }
}

const LinearTaskResults& LinearEquation::get_task_results() const {
    return m_results;
}

void QuadraticEquation::solve() {
    if (!m_solved){
        if (m_task.a != 0) {
            // ax^2 + bx + c = 0
            calculate_discriminant();
            calculate_roots();
            calculate_extremum();
        } else {
            // bx + c = 0
            auto linear_equation = LinearEquation{m_task};
            linear_equation.solve();
            m_results.root1 = linear_equation.get_task_results().root1;
            m_results.root2.reset();
            m_results.extremum = std::nan("0");
        }
        m_solved = true;
    }
}

void QuadraticEquation::calculate_discriminant() {
    m_discriminant = static_cast<int64_t>(m_task.b) * m_task.b - 4 * m_task.a * m_task.c;
}

void QuadraticEquation::calculate_roots() {
    if (m_discriminant > 0) {
        m_results.root1 = (-static_cast<double>(m_task.b) - std::sqrt(m_discriminant)) / (2 * m_task.a);
        m_results.root2 = (-static_cast<double>(m_task.b) + std::sqrt(m_discriminant)) / (2 * m_task.a);
    } else if (m_discriminant == 0) {
        m_results.root1 = -static_cast<double>(m_task.b) / (2 * m_task.a);
        m_results.root2 = m_results.root1;
    } else {
        m_results.root1.reset();
        m_results.root2.reset();
    }
}

void QuadraticEquation::calculate_extremum() {
    LinearTask task = LinearTask();                             // FIXME!
    task.b = m_task.a * 2;
    task.c = m_task.b;
    auto linear_equation = LinearEquation{task};
    linear_equation.solve();
    m_results.extremum = linear_equation.get_task_results().root1.value_or(std::nanf("0"));
}

const QuadraticTaskResults& QuadraticEquation::get_task_results() const {
    return m_results;
}
