// Camera.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Camera.hpp"
#include "Tools.hpp"

#include <iostream>
#include <cmath>

using namespace glm;

namespace stein {

// Default constructor
Camera::Camera() :
    xAxis(1, 0, 0), yAxis(0, 1, 0), zAxis(0, 0, 1), position(0, 0, 1) {
    updateView();
    setPerspectiveProjection(-1, 1, -1, 1, .1, 100);
}

// Updates view
void Camera::updateView() {
    // Rotation to be aligned with correct camera axis
    mat4 RcInv(xAxis.x, yAxis.x, zAxis.x, 0, xAxis.y, yAxis.y, zAxis.y, 0, xAxis.z, yAxis.z, zAxis.z, 0, 0, 0, 0, 1);

    // Translation to be at the right distance from the scene
    mat4 TcInv(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -position.x, -position.y, -position.z, 1);

    // Initializes
    view = RcInv * TcInv;
}

void Camera::setPerspectiveProjection(float left, float right, float bottom, float top, float near, float far) {
    projection = mat4( //
        (2 * near) / (right - left), 0, 0, 0, //
        0, (2 * near) / (top - bottom), 0, 0, (right + left) / (right - left), //
        (top + bottom) / (top - bottom), -(far + near) / (far - near), -1, //
        0, 0, -(2 * far * near) / (far - near), 0);
}

void Camera::setOrthoProjection(float left, float right, float bottom, float top, float near, float far) {
    projection = mat4( //
        2 / (right - left), 0, 0, 0, //
        0, 2 / (top - bottom), 0, 0, //
        0, 0, -2 / (far - near), 0, //
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1);
}

void Camera::setPosition(const vec3&p) {
    position = p;
    updateView();
}

const vec3& Camera::getPosition() const {
    return position;
}

const mat4& Camera::getView() const {
    return view;
}

const mat4& Camera::getProjection() const {
    return projection;
}

} // namespace stein
