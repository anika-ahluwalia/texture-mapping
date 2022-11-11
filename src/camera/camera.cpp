#include <stdexcept>
#include "camera.h"

Camera::Camera(SceneCameraData& camera_data, int width, int height) {
    cam_data = camera_data;
    s_width = width;
    s_height = height;
    view_matrix = createViewMatrix();
    inverse_view_matrix = createInverseViewMatrix();
}

// only creating the camera view matrix once
glm::mat4 Camera::createViewMatrix() {
    glm::vec4 pos = cam_data.pos;
    glm::vec3 look = {cam_data.look[0], cam_data.look[1], cam_data.look[2]};
    glm::vec3 up = {cam_data.up[0], cam_data.up[1], cam_data.up[2]};

    glm::mat4 translate = glm::mat4(1.f, 0.f, 0.f, 0.f,
                                    0.f, 1.f, 0.f, 0.f,
                                    0.f, 0.f, 1.f, 0.f,
                                    -1 *pos[0], -1 * pos[1], -1 * pos[2], 1.f);

    glm::vec3 w = -1.f * glm::normalize(look);
    glm::vec3 v = glm::normalize(up - (dot(up, w)*w));
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

glm::mat4 Camera::getViewMatrix() const {
    return view_matrix;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    return inverse_view_matrix;
}

float Camera::getAspectRatio() const {
    return float(s_width) / float(s_height);
}

float Camera::getHeightAngle() const {
    return cam_data.heightAngle;
}

float Camera::getFocalLength() const {
    return cam_data.focalLength;
}

float Camera::getAperture() const {
    return cam_data.aperture;
}
