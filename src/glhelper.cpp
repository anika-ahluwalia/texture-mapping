#include "glhelper.h"
#include "debug.h"

#include "shapes/cube.h"
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"

GLHelper::GLHelper(int param1, int param2)
{
    m_param1 = param1;
    m_param2 = param2;
}

void GLHelper::cleanMemory() {
    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &sphere_vbo);
    glDeleteVertexArrays(1, &sphere_vao);
    glDeleteBuffers(1, &cylinder_vbo);
    glDeleteVertexArrays(1, &cylinder_vao);
    glDeleteBuffers(1, &cone_vbo);
    glDeleteVertexArrays(1, &cone_vao);
}

void GLHelper::generateAllShapes() {
    generateCubeVBOsVAOs();
    generateConeVBOsVAOs();
    generateCylinderVBOsVAOs();
    generateSphereVBOsVAOs();
}

void GLHelper::generateCubeVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    Cube cube = Cube();
    cube.updateParams(m_param1);
    std::vector<float> cube_vec = cube.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube_vec.size(), &cube_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    // Add position and color attributes to VAO
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void GLHelper::generateSphereVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    Sphere sphere = Sphere();
    sphere.updateParams(m_param1, m_param2);
    std::vector<float> sphere_vec = sphere.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere_vec.size(), &sphere_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void GLHelper::generateCylinderVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cylinder_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_vbo);
    Cylinder cylinder = Cylinder();
    cylinder.updateParams(m_param1, m_param2);
    std::vector<float> cylinder_vec = cylinder.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cylinder_vec.size(), &cylinder_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cylinder_vao);
    glBindVertexArray(cylinder_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void GLHelper::generateConeVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cone_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
    Cone cone = Cone();
    cone.updateParams(m_param1, m_param2);
    std::vector<float> cone_vec = cone.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cone_vec.size(), &cone_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cone_vao);
    glBindVertexArray(cone_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}
