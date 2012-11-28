#ifndef CINETIC_BRAKE_HPP
#define CINETIC_BRAKE_HPP

#include "Spring.hpp"

class CineticBrake : public Spring {
    private:
        float m_z ; // constant
        float m_dt;
    public:
        CineticBrake(float z, float dt):
            m_z(z), m_dt(dt){};

        void generateForces( Particle* p1, Particle* p2 ){
            glm::vec2 v = p2->velocity - p1->velocity;
            p1->force += m_z/m_dt * v;
            p2->force -= m_z/m_dt * v;
        }
};

#endif

