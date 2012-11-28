#ifndef ATTRACTION_HPP
#define ATTRACTION_HPP

#include "Spring.hpp"

class Attraction : public Spring {
    private:
        float m_k; //constante de "gravitation" = 0.00000000006674 unités SI
    public:
        Attraction(float k):m_k(k){};

        void generateForces( Particle* p1, Particle* p2 ){
            glm::vec2 f = m_k * p1->mass * p2->mass * glm::vec2(p2->position - p1->position);
            f /= glm::distance(p1->position, p2->position) * glm::distance(p1->position, p2->position);
            p1->force += f;
            //p2->force -= f;
        }

        /**
            Applique une force d'attraction entre une particle et un polygone
            Le polygone n'est soumis à aucune force.
            p1 : particule
            C : centre de gravité du polygone (voir Polygon::getGCenter())
            M : masse du polygone
        */
        void generateForces( Particle* p1, const glm::vec2& C, float M ){
            glm::vec2 f = m_k * p1->mass * M * glm::vec2(C-p1->position) ;
            f /= glm::distance(p1->position, C) * glm::distance(p1->position, C);
            p1->force += f;
        }

};

#endif

