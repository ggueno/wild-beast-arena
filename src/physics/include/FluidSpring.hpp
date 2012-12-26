#ifndef FLUID_SPRING_HPP
#define FLUID_SPRING_HPP

#include "Spring.hpp"

class FluidSpring : public Spring {
    private:
        float m_k, m_z, m_dt ; // constant

    public:
        FluidSpring(float k, float z, float dt):
            m_k(k), m_z(z){}, m_dt(dt);

        void generateForces( Particle* p1, Particle* p2 ){
            // Apply Hook spring
            float epsilon = 0.00001;
            float d = glm::distance( p1->position, p2->position );

            // Fluid spring detail, l0 depends on the masses
            float l0 = 0.03 * ( p1->mass + p2->mass );
            float delta = 0.01; // TODO check this
            if( d > l0 + delta ) return ;

            float e = 1 - l0 / ( glm::max(epsilon, d) );
            p1->force += m_k * e * glm::vec2(p2->position - p1->position);
            p2->force -= m_k * e * glm::vec2(p2->position - p1->position);

            //Apply Cinetic brake
            glm::vec2 v = p2->velocity - p1->velocity;
            p1->force += m_z/m_dt * v;
            p2->force -= m_z/m_dt * v;
        }
};

#endif

