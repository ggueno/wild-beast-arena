#ifndef POLYGON_SPRING_HPP
#define POLYGON_SPRING_HPP

#include "Spring.hpp"
#include "Polygon.hpp"



/**
 Detecte s'il y a collision pour une particule située en p1 de position future p2 et le segment orienté [A,B].
 Si oui renvoit true et place la position du point d'intersection dans *intersection et la normale en ce point
 dans *normal
*/
inline bool intersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& A, const glm::vec2& B,
	glm::vec2* intersection, glm::vec2* normal) {
	static const float epsilon = 0.001; // Permet de tricher en allongeant la taille des coté

	glm::vec2 dir = p2 - p1; // Vecteur p1p2
	glm::vec2 AB = B - A; // Vecteur AB
	glm::vec2 N = glm::vec2(-AB.y, AB.x); // Vecteur normal non unitaire tel que (AB, N) forme un repère direct

	// Si le vecteur p1p2 et le vecteur N on la meme direction, il n'y a pas d'intersection
        if(glm::dot(dir, N) >= 0) {
            return false;
        }

	// Calcul du parametre t tel que I = p1 + t x p1p2
        float t = -glm::dot(N, p1 - A) / glm::dot(N, dir);

        // Si t n'est pas dans [0,1], le point d'intersection n'est pas dans le segment [p1,p2]
        if(t < 0.f || t > 1) {
            return false;
        }

	glm::vec2 I = p1 + t * dir; // Point d'intersection

	float dot = glm::dot(I - A, AB); // Produit scalaire entre le vecteur AI et le vecteur AB

	// Permet de savoir si le point d'intersection est sur le segment [A - epsilon, B + epsilon]
	if(dot < -epsilon || dot > glm::dot(AB, AB) + epsilon) {
	    return false;
	}

	if(intersection) {
		*intersection = I;
	}

	if(normal) {
		*normal = glm::normalize(N); // On normalise le vecteur normal pour avoir un vecteur unitaire en sortie
	}

	return true;
}


class PolygonSpring: public Spring {

public:
    PolygonSpring(const Polygon& polygon, float elasticity, const LeapFrogSolver& solver, float dt):
        m_Polygon(&polygon), m_fElasticity(elasticity), m_Solver(&solver), m_fDt(dt) {}

    void generateForces(Particle* p, const glm::vec2& v0, const glm::vec2& v1) const ;
    void generateForces(Particle* p1, Particle* p2);

private:
    const Polygon* m_Polygon;
    float m_fElasticity;
    const LeapFrogSolver* m_Solver;
    float m_fDt;
};

#endif

