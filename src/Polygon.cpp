#include "api/Polygon.hpp"
#include <iostream>
/**
    Calcule le centre de gravité d'un polygone, même irrégulier
    D'après : http://math.15873.pagesperso-orange.fr/page9.htm
*/
void Polygon::computeGCenter() {

    // Calcul de l'aire du polygone
    float aire = 0;
    unsigned int vc = m_Vertices.size();
    for( unsigned int i=0 ; i < vc ; ++i ){
        aire += (m_Vertices[i].x * m_Vertices[(i+1)%vc].y) - (m_Vertices[(i+1)%vc].x*m_Vertices[i].y);
        /*printf("(%f*%f)-(%f*%f)=%f\n", m_Vertices[i].x
                                   , m_Vertices[(i+1)%vc].y
                                   , m_Vertices[(i+1)%vc].x
                                   , m_Vertices[i].y
                                   , (m_Vertices[i].x * m_Vertices[(i+1)%vc].y) - (m_Vertices[(i+1)%vc].x*m_Vertices[i].y));
        printf("aire : %f\n", aire);*/

    }
    aire /= 2.0;

    // Calcul de la position du centre de gravité
    float gx=0, gy=0;
    for( unsigned int i=0 ; i < vc ; ++i ){
        gx += ( m_Vertices[i].x + m_Vertices[(i+1)%vc].x ) * ( (m_Vertices[i].x*m_Vertices[(i+1)%vc].y) - (m_Vertices[(i+1)%vc].x*m_Vertices[i].y) );
        gy += ( m_Vertices[i].y + m_Vertices[(i+1)%vc].y ) * ( (m_Vertices[i].x*m_Vertices[(i+1)%vc].y) - (m_Vertices[(i+1)%vc].x*m_Vertices[i].y) );
    }
    gx = gx/(6*aire);
    gy = gy/(6*aire);

    //printf("( %f, %f)\n", gx, gy);
    m_gCenter = glm::vec2(gx, gy);
}

