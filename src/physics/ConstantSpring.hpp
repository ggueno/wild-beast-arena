#ifndef CONSTANT_SPRING_HPP
#define CONSTANT_SPRING_HPP

#include "Spring.hpp"

class ConstantSpring: public Spring {
public:
    ConstantSpring(const Vec2f& force):
        m_Force(force) {}

    void generateForces(Particle* p1, Particle* p2) { p1->force += m_Force; }

private:
    Vec2f m_Force;
};

#endif

