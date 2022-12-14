#include "glhelper.h"
#include "debug.h"

#include "shapes/cube.h"
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"
#include "shapes/building.h"
#include "shapes/tieredbuilding.h"

#include <GL/glew.h>

GLHelper::GLHelper(int param1, int param2)
{
    m_param1 = param1;
    m_param2 = param2;
}

void GLHelper::cleanMemory() {
    glDeleteBuffers(1, &building_vbo);
    glDeleteVertexArrays(1, &building_vao);
}


void GLHelper::generateBuilding(int size, int height, float x, float z) {
    Building building = Building();
    building.updateParams(15, 15, 5, 2, 1, 1, 3);
    building_data = building.getMesh();
    createVAOVBO(building_vbo, building_vao, building_data);

//    TieredBuilding building = TieredBuilding();
//    building.updateParams(1, 2, 5, 1, x, z, 1);
//    building_data = building.getMesh();
//    createVAOVBO(building_vbo, building_vao, building_data);
}

void GLHelper::createVAOVBO(GLuint &vbo, GLuint &vao, std::vector<float> shape_data) {
    // Vertex Buffer Objects //
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape_data.size(), &shape_data[0], GL_STATIC_DRAW);

    // Vertex Array Objects //
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Setting position and normal attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(6 * sizeof(GL_FLOAT)));

    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
