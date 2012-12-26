#ifndef HEALTH_PACK_HPP
#define HEALTH_PACK_HPP

#include "glm/glm.hpp"

class HealthPack : public Polygon {

private:
    float m_scale;
    float m_offset;
    bool m_alive;

public:
    size_t index;// index used in renderer

    HealthPack(float x, float y):Polygon(glm::vec3(1.0, 1.0, 1.0), false), m_scale(0.05), m_offset(0.333), m_alive(true){
        m_offset *= m_scale;
        float width = 1.0*m_scale;
        addVertex( glm::vec2(x-m_offset, y-width) );
        addVertex( glm::vec2(x-m_offset, y-m_offset) );
        addVertex( glm::vec2(x-width, y-m_offset) );
        addVertex( glm::vec2(x-width, y+m_offset) );
        addVertex( glm::vec2(x-m_offset, y+m_offset) );
        addVertex( glm::vec2(x-m_offset, y+width) );
        addVertex( glm::vec2(x+m_offset, y+width) );
        addVertex( glm::vec2(x+m_offset, y+m_offset) ) ;
        addVertex( glm::vec2(x+width, y+m_offset) );
        addVertex( glm::vec2(x+width, y-m_offset) );
        addVertex( glm::vec2(x+m_offset, y-m_offset) );
        addVertex( glm::vec2(x+m_offset, y-width) );
        m_gCenter = glm::vec2( x, y );
    }

    void setAlive(bool alive){ m_alive = false;};
    void kill(){ m_alive = false; };
    bool isAlive(){ return m_alive; };


};


#endif
