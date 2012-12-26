#include "LeapFrogSolver.hpp"

void LeapFrogSolver::solve( Particle &p, float step ) const {
    p.velocity += step * p.force / p.mass;
    p.position += step * p.velocity;
    p.force = glm::vec2(0,0);
}


void LeapFrogSolver::getNextState(const Particle& particle, float dt, Particle& next) const {
    next.velocity = particle.velocity + dt * particle.force / particle.mass;
    next.position = particle.position + dt * next.velocity;
}
