#include "glhelper.h"
#include "debug.h"

#include "shapes/cube.h"
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"

#include <GL/glew.h>

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
    for (int i = 1; i < 5; i++) {
        switch (i) {
            case 1: {
                Cylinder cylinder = Cylinder();
                cylinder.updateParams(m_param1, std::max(m_param2, 3));
                cylinder_data = cylinder.generateShape();
                makeShape(cylinder_vbo, cylinder_vao, cylinder_data);
                break;
            }
            case 2: {
                Sphere sphere = Sphere();
                sphere.updateParams(std::max(m_param1, 2), std::max(m_param2, 3));
                sphere_data = sphere.generateShape();
                makeShape(sphere_vbo, sphere_vao, sphere_data);
                break;
            }
            case 3: {
                Cube cube = Cube();
                cube.updateParams(m_param1);
                cube_data = cube.generateShape();
                makeShape(cube_vbo, cube_vao, cube_data);
                break;
            }
            case 4: {
                Cone cone = Cone();
                cone.updateParams(m_param1, std::max(m_param2, 3));
                cone_data = cone.generateShape();
                makeShape(cone_vbo, cone_vao, cone_data);
                break;
            }
        }
    }
}

void GLHelper::makeShape(GLuint &vbo, GLuint &vao, std::vector<float> shape_data) {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
