#ifndef _API_SPRING_HPP_
#define _API_SPRING_HPP_

#include "glm/glm.hpp"

#include "Particle.hpp"
#include "LeapFrogSolver.hpp"


class Spring {
public:
    virtual ~Spring(){}
    virtual void generateForces( Particle *p1, Particle *p2 )=0; // pure virtual func

};


#endif

