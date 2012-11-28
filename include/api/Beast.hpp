#ifndef BEAST_HPP
#define BEAST_HPP

#include "api/Particle.hpp"


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
        const static int PV_MAX = 20;
        //const float VAR_AGGRO = 0.3;
        
        Beast(){ m_life = PV_MAX;}; // default constructor, should not use it.
        Beast( glm::vec2 position ):Particle(position){};

        void collide( Beast* b ){
            touched();
            b->touched();
        }

        void touched(){ 
            --m_life; // decrease life
            //m_aggro*=1.1; // an injured animal is more dangerous
         }
}
        
        
#endif
