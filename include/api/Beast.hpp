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

    public :
        int m_life;     // life points
        float m_aggro;  // percentage of aggressiveness

    public:
        static const int PV_MAX = 100000;

        // CONSTRUCTORS
        Beast():Particle( 1.0 ) { // default constructor, should not use it directly.
            m_life = PV_MAX;
            m_aggro = rand()/(float)RAND_MAX;
            color[0] = 1.0;//m_aggro;
            color[1] = 1.0;
            color[2] = 1.0;
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
            m_life  -= 10*coef;                         // decrease life
            m_aggro  = glm::min(0.0001+m_aggro, 1.0);   // an injured animal is more dangerous
            //color[0] = m_life;
            //color[2]-= m_aggro; // slightly change color to red
            //color[1]-= 0.001*color[1];
         }

  
        void addForce( const glm::vec2& f ){ force+=f;}

};

#endif
