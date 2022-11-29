#include <ostream>
#include <stdexcept>
#include "camera.h"
#include <iostream>

Camera::Camera(SceneCameraData& camera_data, int width, int height, float nearPlane, float farPlane) {
    // cam_data = camera_data;
    s_width = width;
    s_height = height;
    near_plane = nearPlane;
    far_plane = farPlane;

    heightAngle = camera_data.heightAngle;

    pos = camera_data.pos;
    look = camera_data.look;
    up = camera_data.up;

    view_matrix = createViewMatrix();
    inverse_view_matrix = createInverseViewMatrix();
    projection_matrix = createProjectionMatrix();
}

// only creating the camera view matrix once
glm::mat4 Camera::createViewMatrix() {

    glm::mat4 translate = glm::mat4(1.f, 0.f, 0.f, 0.f,
                                    0.f, 1.f, 0.f, 0.f,
                                    0.f, 0.f, 1.f, 0.f,
                                    -1 *pos[0], -1 * pos[1], -1 * pos[2], 1.f);

    glm::vec3 w = -1.f * glm::normalize(glm::vec3(look));
    glm::vec3 v = glm::normalize(glm::vec3(up) - (dot(glm::vec3(up), w)*w));
    glm::vec3 u = cross(v, w);

    glm::mat4 rotate = glm::mat4(u[0], v[0], w[0], 0.f,
                                 u[1], v[1], w[1], 0.f,
                                 u[2], v[2], w[2], 0.f,
                                 0.f, 0.f, 0.f, 1.f);

    return rotate * translate;
}

// only creating the inverse view matrix once
glm::mat4 Camera::createInverseViewMatrix() {
    return inverse(view_matrix);
}

glm::mat4 Camera::createProjectionMatrix() {
    float c = -1.f * near_plane / far_plane;

    glm::mat4 transformation = glm::mat4(1.f, 0.f,  0.f,  0.f,
                                         0.f, 1.f,  0.f,  0.f,
                                         0.f, 0.f, -2.f,  0.f,
                                         0.f, 0.f, -1.f,  1.f);

    glm::mat4 unhinging = glm::mat4(1.f, 0.f,       0.f,       0.f,
                                    0.f, 1.f,       0.f,       0.f,
                                    0.f, 0.f, 1.f/(1.f + c),  -1.f,
                                    0.f, 0.f,  -c/(1.f + c),   0.f);

    float widthAngle = getHeightAngle() * getAspectRatio();
    float val1 = 1.f / (far_plane * glm::tan(widthAngle / 2.f));
    float val2 = 1.f / (far_plane * glm::tan(getHeightAngle() / 2.f));
    float val3 = 1.f / far_plane;

    glm::mat4 scaling = glm::mat4(val1,  0.f,   0.f,  0.f,
                                   0.f, val2,   0.f,  0.f,
                                   0.f,  0.f,  val3,  0.f,
                                   0.f,  0.f,   0.f,  1.f);

    return transformation * unhinging * scaling;
}

void Camera::translate(glm::vec3 translation) {
    pos = pos + glm::vec4(translation, 0);
    view_matrix = createViewMatrix();
    inverse_view_matrix = createInverseViewMatrix();
}

void Camera::rotate(glm::vec4 look, glm::vec4 up) {
//    look = look;
//    up = up;
    view_matrix = createViewMatrix();
    inverse_view_matrix = createInverseViewMatrix();
}

glm::vec3 Camera::getPos() {
    return pos;
}
glm::vec3 Camera::getLook() {
    return look;
}
glm::vec3 Camera::getUp() {
    return up;
}

glm::mat4 Camera::getViewMatrix() const {
    return view_matrix;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    return inverse_view_matrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projection_matrix;
}

float Camera::getAspectRatio() const {
    return float(s_width) / float(s_height);
}

float Camera::getHeightAngle() const {
    return heightAngle;
}
