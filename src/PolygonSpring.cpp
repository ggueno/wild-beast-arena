#include "api/PolygonSpring.hpp"


void PolygonSpring::generateForces(Particle* p1, Particle* p2) {
    // si le polygone est interne, on parcourt les sommets dans l'ordre trigonométrique
    if(m_Polygon->isInner()) {
        for(int i = 0; i < m_Polygon->size(); ++i) {
            generateForces(p1, m_Polygon->getVertex(i), m_Polygon->getVertex((i + 1) % m_Polygon->size()));
        }
    } 
	// Sinon dans l'ordre contraire
	else {
        for(int i = m_Polygon->size() - 1; i >= 0; --i)
            generateForces(p1, m_Polygon->getVertex(i), m_Polygon->getVertex((i - 1 + m_Polygon->size()) % m_Polygon->size()));
    }
}



// Génère une force s'il a collision avec le coté [v0,v1]
void PolygonSpring::generateForces(Particle* p, const glm::vec2& v0, const glm::vec2& v1) const {
    Particle next;
    m_Solver->getNextState(*p, m_fDt, next);
    glm::vec2 intersection, normal;

	// si intersection, ajout de la force (voir "Force de collision" plus haut)
    if(intersect(p->position, next.position, v0, v1, &intersection, &normal)) {
        float alpha = glm::dot(next.velocity, -normal) * p->mass / m_fDt;
        p->force += m_fElasticity * alpha * normal;
    }
}


