// Camera.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "../glm/glm.hpp"

namespace stein {

// A camera to watch the scene
struct Camera {
    Camera();

    void setPosition(const glm::vec3&);
    void setPerspectiveProjection(float left, float right, float bottom, float top, float near, float far);
    void setOrthoProjection(float left, float right, float bottom, float top, float near, float far);

    const glm::vec3& getPosition() const;
    const glm::mat4& getView() const;
    const glm::mat4& getProjection() const;
private:
    void updateView();

    const glm::vec3 xAxis; // Camera axis x : right side
    const glm::vec3 yAxis; // Camera axis y : up
    const glm::vec3 zAxis; // Camera axis z : backward

    glm::vec3 position; // Camera position
    glm::mat4 view; // View matrix
    glm::mat4 projection; // Projection matrix
};

} // namespace stein

#endif // __CAMERA_HPP__
