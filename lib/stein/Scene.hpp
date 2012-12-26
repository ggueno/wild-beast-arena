// Scene.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "GLHeaders.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "../glm/glm.hpp"

#include <vector>

namespace stein {

// Forward declaration
struct Object;

// A container for objects
class Scene {
    struct ObjectInstance {
        int objectId;
        size_t shaderId;
        glm::mat4 transformation;
        Color color;
        GLuint textureId0;
        GLuint textureId1;

        ObjectInstance(size_t objectId, size_t shaderId, GLuint textureId0, GLuint textureId1, const glm::mat4& transformation, const Color&color) :
            objectId(objectId), shaderId(shaderId), transformation(transformation), color(color), textureId0(textureId0),  textureId1(textureId1){
        }
    };

    std::vector<Object*> storedObjects; // Library of Objects to use from GPU
    std::vector<ObjectInstance> drawnObjects;

    Color defaultColor; // Default color for drawn elements
    glm::mat4 defaultTransformation; // Default transformation matrix for drawn elements
    GLuint defaultShaderID; // Default shaderID for drawn elements
    GLuint defaultTextureID0; // Default shaderID for drawn elements
    GLuint defaultTextureID1; // Default shaderID for drawn elements
    GLfloat lightPosition[4]; // Position of the light used in shader
    GLfloat lightPower; // Power of the light used in shader
public:
    const static size_t maxStoredObjects = 50; // An initial limit of storable objects
    const static size_t maxDrawnObjects = 200; // An initial limit of drawable objects

    Scene();
    ~Scene();

    Camera camera; // Camera used to watch the scene

    Object& createObject(GLenum primitiveType);
    GLuint addObjectToDraw(GLuint indexStoredObject);
    bool deleteDrawnObject(GLuint indexDrawnObject);

    void setDrawnObjectColor(GLuint indexDrawnObject, const Color &color);
    void setDrawnObjectModel(GLuint indexDrawnObject, const glm::mat4 &model);
    void setDrawnObjectShaderID(GLuint indexDrawnObject, GLuint shaderID);
    void setDrawnObjectTextureID0(GLuint indexDrawnObject, GLuint textureID);
    void setDrawnObjectTextureID1(GLuint indexDrawnObject, GLuint textureID);

    void setDefaultColor(const Color &defaultColor);
    void setDefaultModel(const glm::mat4 &defaultModel);
    void setDefaultShaderID(GLuint defaultShaderID);
    void setDefaultTextureID0(GLuint id);
    void setDefaultTextureID1(GLuint id);

    glm::mat4 getDrawnObjectModel(GLuint indexDrawnObject){ return drawnObjects[indexDrawnObject].transformation; }

    void setLight(GLfloat * position, GLfloat power);

    void drawObjectsOfScene(int timer);
private:
    void setAppearance(const ObjectInstance &, int timer);
};

} // namespace stein

#endif // __SCENE_HPP__
