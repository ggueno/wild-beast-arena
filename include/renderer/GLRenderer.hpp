#ifndef _GLRENDERER_HPP_
#define _GLRENDERER_HPP_

#include <vector>
#include <GL/glew.h>
#include "api/Particle.hpp"

class GLRenderer {
public:
    GLRenderer(GLfloat massScaleFactor = 0.05f);

    ~GLRenderer();

    void render(const Particle* particles, size_t count) const;

    size_t addPolygon(const glm::vec2* vertices, size_t count, const glm::vec3& color);

    void removePolygon(size_t index);

    void displayPolygon(size_t index, bool display = true);

private:
    static const GLuint POSITION_ATTRIB_LOCATION = 0;

    GLfloat m_MassScaleFactor; // The size of a particle is m_MassScaleFactor times its mass
    GLuint m_ParticleVBO;
    GLuint m_ParticleVAO;
    GLuint m_ParticleProgram;
    GLint m_ParticleColorUniformLocation;
    GLint m_ParticlePositionUniformLocation;
    GLint m_ParticleScaleUniformLocation;

    std::vector<bool> m_PolygonIsEnabled;
    std::vector<size_t> m_PolygonSize;
    std::vector<GLuint> m_PolygonVBOs;
    std::vector<GLuint> m_PolygonVAOs;
    std::vector<glm::vec3> m_PolygonColors;
    GLuint m_PolygonProgram;
    GLint m_PolygonColorUniformLocation;
};

#endif
