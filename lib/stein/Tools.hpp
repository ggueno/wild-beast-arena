// Tools.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __TOOLS_HPP__
#define __TOOLS_HPP__

// To include GL
#include "GLHeaders.hpp"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"


// C++
#include <string>
#include <vector>

namespace stein {

glm::mat4 translation(const glm::vec3&t);
glm::mat4 scalee(const glm::vec3&s);
glm::mat4 xRotation(const float angle);
glm::mat4 yRotation(const float angle);
glm::mat4 zRotation(const float angle);
glm::mat4 rotation(const float angle, const glm::vec3 &axis);

void printGlErrors();

GLuint loadTexture(const char* fileName);
GLuint loadProgram(const std::vector<std::string> & files);
GLuint loadProgram(const std::string& file);
void printShaderLog(GLint shaderId);

std::string * loadFile(const std::string & fileName);
unsigned char * loadPPM(const char* filename, GLuint &width, GLuint &height);

void setMatricesInShader(GLuint shaderId, const glm::mat4 &model, const glm::mat4 &view, const glm::vec3 &eye, const glm::mat4 &projection);
void setLightInShader(GLuint shaderID, GLfloat * position, GLfloat power);
void setMaterialInShader(GLuint shaderID, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat shininess);
void setFilledDataInShader(GLuint shaderID, GLboolean positions, GLboolean normals, GLboolean uvs, GLboolean colors);
void setTextureUnitsInShader(GLuint shaderID);
void setTimerInShader(GLuint shaderID, GLfloat timer);


void changeMaterialColorInShader(GLuint shaderID, GLfloat * color);

uint64_t getTime();

} // namespace stein

#endif //  __TOOLS_HPP__
