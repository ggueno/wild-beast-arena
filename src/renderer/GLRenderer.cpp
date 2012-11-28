#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <iterator>

#include <glm/gtc/matrix_transform.hpp>

#include "renderer/GLRenderer.hpp"

static const char* readFile(const char* filePath) {
    std::ifstream file(filePath);

    if (!file) {
        return 0;
    }

    file.seekg(0, std::ios_base::end);
    size_t length = file.tellg();
    file.seekg(0);

    char* src = new char[length + 1];
    file.read(src, length);
    src[file.tellg()] = '\0';

    return src;
}

GLuint loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    const char* vertexShaderSource = readFile(vertexShaderFile);
    if(!vertexShaderSource) {
        std::cerr << "Unable to load " << vertexShaderFile << std::endl;
        return 0;
    }

    const char* fragmentShaderSource = readFile(fragmentShaderFile);
    if(!fragmentShaderSource) {
        std::cerr << "Unable to load " << fragmentShaderFile << std::endl;
        return 0;
    }

    // Creation d'un Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Spécification du code source
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);

    // Compilation du shader
    glCompileShader(vertexShader);

    /// Vérification que la compilation a bien fonctionnée (très important !)
    GLint compileStatus;

    // Récupération du status de compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        char* log = new char[logLength];

        glGetShaderInfoLog(vertexShader, logLength, 0, log);
        std::cerr << "Vertex Shader error:" << log << std::endl;
        std::cerr << vertexShaderSource << std::endl;

        delete [] log;
        return 0;
    }

    // Creation d'un Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Spécification du code source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

    // Compilation du shader
    glCompileShader(fragmentShader);

    /// Vérification que la compilation a bien fonctionnée (très important !)

    // Récupération du status de compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de compilation
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        char* log = new char[logLength];

        glGetShaderInfoLog(fragmentShader, logLength, 0, log);
        std::cerr << "Fragment Shader error:" << log << std::endl;
        std::cerr << fragmentShaderSource << std::endl;

        delete [] log;
        return 0;
    }

    GLuint program;

    // Creation d'un programme
    program = glCreateProgram();

    // Attachement des shaders au programme
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Désallocation des shaders: ils ne seront réellement supprimés que lorsque le programme sera supprimé
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Edition de lien
    glLinkProgram(program);

    /// Vérification que l'édition de liens a bien fonctionnée (très important aussi !)
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE) {
        // Si echec, récupération de la taille du log de link
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        // Allocation d'une chaine de caractère suffisement grande pour contenir le log
        char* log = new char[logLength];

        glGetProgramInfoLog(program, logLength, 0, log);
        std::cerr << "Program link error:" << log << std::endl;

        delete [] log;
        return 0;
    }

    delete [] vertexShaderSource;
    delete [] fragmentShaderSource;

    return program;
}

static const char *PARTICULE_VERTEX_SHADER = "shaders/particle.vs.glsl";
static const char *PARTICULE_FRAGMENT_SHADER = "shaders/particle.fs.glsl";

static const char *POLYGON_VERTEX_SHADER = "shaders/polygon.vs.glsl";
static const char *POLYGON_FRAGMENT_SHADER = "shaders/polygon.fs.glsl";

GLRenderer::GLRenderer(GLfloat massScaleFactor):
    m_MassScaleFactor(massScaleFactor) {
    // Init glew (A GL context must exist)
    GLenum error;
    if((error = glewInit()) != GLEW_OK) {
        throw std::runtime_error((const char*)glewGetErrorString(error));
    }

    m_ParticleProgram = loadProgram(PARTICULE_VERTEX_SHADER, PARTICULE_FRAGMENT_SHADER);
    if(!m_ParticleProgram) {
        throw std::runtime_error("Particle shaders loading has failed");
    }

    m_ParticleColorUniformLocation = glGetUniformLocation(m_ParticleProgram, "uParticleColor");
    if(m_ParticleColorUniformLocation < 0) {
        throw std::runtime_error("No uniform variable called uParticleColor in the particle shaders");
    }

    m_ParticlePositionUniformLocation = glGetUniformLocation(m_ParticleProgram, "uParticlePosition");
    if(m_ParticlePositionUniformLocation < 0) {
        throw std::runtime_error("No uniform variable called uParticlePosition in the particle shaders");
    }

    m_ParticleScaleUniformLocation = glGetUniformLocation(m_ParticleProgram, "uParticleScale");
    if(m_ParticleColorUniformLocation < 0) {
        throw std::runtime_error("No uniform variable called uParticleScale in the particle shaders");
    }


    m_PolygonProgram = loadProgram(POLYGON_VERTEX_SHADER, POLYGON_FRAGMENT_SHADER);
    if(!m_PolygonProgram) {
        throw std::runtime_error("Polygon shaders loading has failed");
    }

    m_PolygonColorUniformLocation = glGetUniformLocation(m_PolygonProgram, "uPolygonColor");
    if(m_PolygonColorUniformLocation < 0) {
        throw std::runtime_error("No uniform variable called uPolygonColor in the polygon shaders");
    }

    glGenBuffers(1, &m_ParticleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);

    // A particle is a square
    GLfloat positions[] = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,
        -1.f,  1.f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_ParticleVAO);
    glBindVertexArray(m_ParticleVAO);

    glEnableVertexAttribArray(POSITION_ATTRIB_LOCATION);
    glVertexAttribPointer(POSITION_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

GLRenderer::~GLRenderer() {
    glDeleteBuffers(1, &m_ParticleVBO);
    glDeleteVertexArrays(1, &m_ParticleVAO);

    for(size_t i = 0; i < m_PolygonVBOs.size(); ++i) {
        removePolygon(i);
    }
}

size_t GLRenderer::addPolygon(const glm::vec2* vertices, size_t count, const glm::vec3& color) {
    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
        glEnableVertexAttribArray(POSITION_ATTRIB_LOCATION);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * count, vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(POSITION_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_PolygonVBOs.push_back(vbo);
    m_PolygonVAOs.push_back(vao);
    m_PolygonColors.push_back(color);
    m_PolygonIsEnabled.push_back(true);
    m_PolygonSize.push_back(count);

    return m_PolygonVBOs.size() - 1;
}

void GLRenderer::removePolygon(size_t index) {
    if(!m_PolygonVBOs[index]) {
        return;
    }
    glDeleteBuffers(1, &m_PolygonVBOs[index]);
    glDeleteVertexArrays(1, &m_PolygonVAOs[index]);
    m_PolygonVBOs[index] = 0;
    m_PolygonVAOs[index] = 0;
}

void GLRenderer::displayPolygon(size_t index, bool display) {
    m_PolygonIsEnabled[index] = display;
}

void GLRenderer::render(const Particle* particles, size_t count) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_ParticleProgram);

    glBindVertexArray(m_ParticleVAO);

    for(const Particle *p = particles, *end = particles + count;
        p != end; ++p) {
        glUniform3f(m_ParticleColorUniformLocation, p->color.r, p->color.g, p->color.b);
        glUniform2f(m_ParticlePositionUniformLocation, p->position.x, p->position.y);
        glUniform1f(m_ParticleScaleUniformLocation, m_MassScaleFactor * p->mass);

        const size_t vertexCount = 4;
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    }

    glBindVertexArray(0);

    glDisable(GL_BLEND);

    glUseProgram(m_PolygonProgram);

    for(size_t i = 0; i < m_PolygonVBOs.size(); ++i) {
        if(m_PolygonIsEnabled[i]) {
            glUniform3f(m_PolygonColorUniformLocation, m_PolygonColors[i].r, m_PolygonColors[i].g, m_PolygonColors[i].b);
            glBindVertexArray(m_PolygonVAOs[i]);
            glDrawArrays(GL_LINE_LOOP, 0, m_PolygonSize[i]);
        }
    }
    glBindVertexArray(0);
}
