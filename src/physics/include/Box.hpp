#ifndef BOX_HPP
#define BOX_HPP

#include <glm/glm.hpp>
#include "Polygon.hpp"

class Box {
public:
    static void build(const glm::vec2& position, float width, float height, Polygon& polygon) {
        polygon.addVertex(position);
        polygon.addVertex(position + glm::vec2(width, 0));
        polygon.addVertex(position + glm::vec2(width, height));
        polygon.addVertex(position + glm::vec2(0, height));
    }
};


#endif

