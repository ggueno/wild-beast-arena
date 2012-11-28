#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <glm/glm.hpp>
#include <vector>

class Polygon {
private:
    bool m_bInner;

public:
    glm::vec3 color;
    std::vector<glm::vec2> m_Vertices;

    Polygon(const glm::vec3& col, bool inner = true):
        m_bInner(inner) {
        color = col;
    }

    void addVertex(const glm::vec2& vertex) {
        m_Vertices.push_back(vertex);
    }

    const glm::vec2& getVertex(size_t i) const {
        return m_Vertices[i];
    }

    size_t size() const {
        return m_Vertices.size();
    }

    bool isInner() const {
        return m_bInner;
    }

    const std::vector<glm::vec2>& getVertices() const {
        return m_Vertices;
    }

    const glm::vec2& getGCenter() const ;
};





#endif

