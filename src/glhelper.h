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
    void generateAllShapes();

    void generateCubeVBOsVAOs();
    void generateSphereVBOsVAOs();
    void generateCylinderVBOsVAOs();
    void generateConeVBOsVAOs();

private:
    int m_param1;
    int m_param2;

    GLuint cube_vbo;       //Stores id for cube vbo
    GLuint cube_vao;       //Stores id for cube vao
    GLuint sphere_vbo;     //Stores id for sphere vbo
    GLuint sphere_vao;     //Stores id for sphere vao
    GLuint cylinder_vbo;   //Stores id for cylinder vbo
    GLuint cylinder_vao;   //Stores id for cylinder vao
    GLuint cone_vbo;       //Stores id for cone vbo
    GLuint cone_vao;       //Stores id for cone vao
};

#endif // GLHELPER_H
