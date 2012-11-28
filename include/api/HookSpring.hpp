#ifndef HOOK_SPRING_HPP
#define HOOK_SPRING_HPP

#include "Spring.hpp"

class HookSpring : public Spring {
    private:
        float m_k ; // constant
        float m_L0 ;
    public:
        HookSpring(float k, float l0):
            m_k(k), m_L0(l0){};

        void generateForces( Particle* p1, Particle* p2 ){
            float epsilon = 0.00001;
            float d = glm::distance( p1->position, p2->position );
            float e = 1 - m_L0 / ( glm::max(epsilon, d) );
            p1->force += m_k * e * glm::vec2(p2->position - p1->position);
            p2->force -= m_k * e * glm::vec2(p2->position - p1->position);
        }


};

#endif

