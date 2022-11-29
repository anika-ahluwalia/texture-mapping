#pragma once

#include <glm/glm.hpp>
#include "utils/scenedata.h"

// A class representing a virtual camera.

class Camera {
public:
    Camera(SceneCameraData& camera_data, int width, int height, float nearPlane, float farPlane);

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 getViewMatrix() const;
    glm::mat4 getInverseViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    void translate(glm::vec4 translation);
    void rotate(glm::vec4 look, glm::vec4 up);
    // SceneCameraData getCamData();
    glm::vec4 getPos();
    glm::vec4 getLook();
    glm::vec4 getUp();

protected:
    glm::mat4 createViewMatrix();
    glm::mat4 createInverseViewMatrix();
    glm::mat4 createProjectionMatrix();

private:
    // SceneCameraData cam_data;
    int s_width;
    int s_height;
    float near_plane;
    float far_plane;
    glm::mat4 view_matrix;
    glm::mat4 inverse_view_matrix;
    glm::mat4 projection_matrix;

    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle;
};
