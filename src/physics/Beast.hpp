#ifndef BEAST_HPP
#define BEAST_HPP

#include "Particle.hpp"
#include <math.h>
#include <cstdlib>
#include <iostream>

/**
    This class defines a particular type of Particles that are born
    to fight in a closed arena. They lose a life point as they collide,
    search for weaker beasts in the neighbourhood and flee if spotted
    by a stronger enemy.
*/
class Beast : public Particle {

    public :
        int m_life;     // life points
        float m_aggro;  // percentage of aggressiveness
        void updateColor(){ color[0] = m_life/(float)PV_MAX; color[2] = 0.0; color[1]=m_aggro; }
        size_t index;

    public:
        static const int PV_MAX = 100;

        // CONSTRUCTORS
        Beast():Particle( 1.0 ) { // default constructor, should not use it directly.
            m_life = PV_MAX;
            m_aggro = rand()/(float)RAND_MAX;
            updateColor();
        };

        // GETTERS & SETTERS
        int life() const { return m_life ; }
        float aggro() const { return m_aggro; }
        bool isAlive() const { return (m_life > 0); }


        // METHODS
        void collide( Beast* b ){
            touched( b->aggro() );
            b->touched( m_aggro );
        }

        void touched(float coef){

            m_life  -= coef;                         // decrease life
            m_aggro  = glm::min(0.0001+m_aggro, 1.0);   // an injured animal is more dangerous
            updateColor();
         }

        void heal(){ m_life += 0.2 * PV_MAX; updateColor(); }

        void addForce( const glm::vec2& f ){ force+=f;}//if(glm::length(force)>20.0)fprintf(stderr, "%f\t", glm::length(force) );}

};


#endif
