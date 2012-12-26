#ifndef _API_GRAVITY_HPP_
#define _API_GRAVITY_HPP_

#include "Spring.hpp"

class GravitySpring:public Spring{
public:
    glm::vec2 force;

    GravitySpring(float g): force(0.0,-g){};

    void generateForces(Particle *p1, Particle *useless){
        p1->force += p1->mass*force;
    }
};
#endif

