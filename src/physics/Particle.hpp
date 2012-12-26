#ifndef _API_PARTICLE_HPP_
#define _API_PARTICLE_HPP_

#include "glm/glm.hpp"

class Particle {

public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 force;
    glm::vec3 color;
    float mass;


    Particle( float mass=0 ):position(0,0)
                            , velocity(0,0)
                            , force(0,0)
                            , color(1.0,1.0,1.0)
                            , mass(mass){};

    Particle( glm::vec2 pos, float mass=0 ):position(pos)
                                            , velocity(0,0)
                                            , force(0,0)
                                            , color(1.0,1.0,1.0)
                                            , mass(mass){};

    Particle( glm::vec3 color, float mass=0 ): position(0,0)
                                            , velocity(0,0)
                                            , force(0,0)
                                            , color(color)
                                            , mass(mass){};


};

#endif

