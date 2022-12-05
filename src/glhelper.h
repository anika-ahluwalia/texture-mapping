#pragma once

#ifndef GLHELPER_H
#define GLHELPER_H

#include "utils/sceneparser.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class GLHelper
{
public:
    GLHelper(int param1, int param2);
    void cleanMemory();
    void createVAOVBO(GLuint &vbo, GLuint &vao, std::vector<float> shape_data);
    void generateBuilding(int size, int height);
    GLuint building_vbo;
    GLuint building_vao;
    std::vector<float> building_data;

private:
    int m_param1;
    int m_param2;
};

#endif // GLHELPER_H
