#ifndef BEAST_SPRING_HPP
#define BEAST_SPRING_HPP

#include "Spring.hpp"
#include <math.h>


class BeastSpring : public Spring {
    private:
        float m_alpha ; // random flee angle, in degrees
        float m_dt ;
    public:
        BeastSpring(float alpha, float dt):
            m_alpha(alpha), m_dt(dt){};

        void generateForces( Particle* p1, Particle* p2 ){
            // First vector is simple
            glm::vec2 attack( p2->position - p1->position );
            attack = attack* m_dt;
            // We add a random angle to make the second more complicated
            float theta = (rand() / RAND_MAX) * m_alpha * 2 - 1;
            theta = glm::degtorad(theta); // degree to radian conversion
            float cs = cos(theta);
            float sn = sin(theta);
            glm::vec2 run( -(attack.x*cs - attack.y*sn)
                         , -(attack.x*sn + attack.y*cs); 

            if( b1->life > b2->life() ){
                b1->force += attack * b1->aggro * ()b1->life()-b2->life())*0.01);
                b2->force += run ; // TODO define a better behaviour
            }
            else {
                b2->force += attack * b2->aggro * ()b2->life()-b1->life())*0.01);
                b1->force += run ; // TODO define a better behaviour
            }
        }


};

#endif

