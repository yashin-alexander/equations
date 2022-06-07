#ifndef EQUATION_H
#define EQUATION_H

class Equation {
public:
    virtual ~Equation() = default;
    virtual void solve() = 0;
private:
};

class LinearEquation : public Equation {
public:
    LinearEquation(int a, int b) : m_a(a), m_b(b) {};
    void solve() override;
private:
    int m_a;
    int m_b;
};

class QuadraticEquation : public Equation {
public:
    QuadraticEquation(int a, int b, int c) : m_a(a), m_b(b), m_c(c) {};
    void solve() override;
private:
    void calculate_discriminant();
    int m_a;
    int m_b;
    int m_c;
};


#endif // EQUATION_H
