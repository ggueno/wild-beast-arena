#ifndef BEAST_HPP
#define BEAST_HPP

#include "api/Particle.hpp"
#include <math.h>

/**
    This class defines a particular type of Particles that are born
    to fight in a closed arena. They lose a life point as they collide,
    search for weaker beasts in the neighbourhood and flee if spotted
    by a stronger enemy.
*/
class Beast : public Particle {

    protected :
        int m_life;     // life points
        float m_aggro;  // aggressiveness

    public:
        static const int PV_MAX = 20;
        static const int VAR_AGGRO = 10;
        
        // CONSTRUCTORS
        Beast():Particle(2.0) { // default constructor, should not use it directly.
            m_life = PV_MAX;
            m_aggro = (rand()/RAND_MAX)*VAR_AGGRO;
        };
        //Beast( glm::vec2 position ):Particle( position ){};

        // GETTERS & SETTERS
        int life(){ return m_life ; }
        float aggro(){ return m_aggro; }

        void collide( Beast* b ){
            touched();
            b->touched();
        }

        void touched(){ 
            //--m_life; // decrease life
            //mass-=0.01;
            m_aggro*=1.1; // an injured animal is more dangerous
         }

        bool isAlive(){ return (m_life > 0 || mass <= 0); }
};
        
#endif
