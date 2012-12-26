#ifndef _API_LFSOLVER_HPP_
#define _API_LFSOLVER_HPP_

#include "Particle.hpp"

class LeapFrogSolver {
public:
    void solve( Particle &p, float step ) const;
    void getNextState(const Particle& particle, float dt, Particle& next) const;
};

#endif

