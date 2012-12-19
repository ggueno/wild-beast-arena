#ifndef BEAST_SPRING_HPP
#define BEAST_SPRING_HPP

#include "Spring.hpp"
#include "Beast.hpp"

#include <math.h>

float degToRad( float angle ){ return angle*3.1415927 / 180.0 ; } // Ouh c'est pas propre, TODO à déplacer

class BeastSpring : public Spring {
    private:
        float m_alpha ;     // random flee angle, in degrees
        float m_threshold ; // security distance threshold
        float m_dt ;        // simulation step

    public:
        BeastSpring(float alpha, float threshold, float dt):
            m_alpha(alpha), m_threshold(threshold), m_dt(dt){srand(time(NULL));};

        void generateForces( Particle* p1, Particle* p2 ){
            generateForces( (Beast*)p1, (Beast*)p2 );
        }

        void generateForces( Beast* p1, Beast* p2 ){

            // Proximity handling
            float distance = glm::distance( p1->position, p2->position );
            if( distance < m_threshold ) p1->collide( p2 ); // En cas de contact, un comportement à définir

            // New forces
            // First vector is simple
            //float epsilon = 0.00001;
            glm::vec2 attack( p2->position - p1->position );
            float e = (m_threshold+distance) / m_threshold ; // 1 < e < ~2
            attack = attack * e ;

            // We add a random angle to make the second more complicated
            float theta = degToRad(m_alpha); // degree to radian conversion
            theta = ( (rand() / (float)RAND_MAX) * 2.0 - 1.0 ) * theta;

            float cs = cos(theta);
            float sn = sin(theta);
            glm::vec2 run( (attack.x*cs - attack.y*sn)
                         , (attack.x*sn + attack.y*cs) );

            if( p1->life()*p1->aggro() > p2->life() ){
                // coef is used to adjust haste
                float coef = p1->aggro() ;
                p1->addForce( attack * coef );
                p2->addForce( run * coef  );
            }
            else {
                float coef = p2->aggro() ;
                p2->addForce( -attack * coef );
                p1->addForce( -run * coef );

            }
        }


};

#endif

