#include "Cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Cylinder::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topLeft[0], 0, topLeft[2])));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomLeft[0], 0, bottomLeft[2])));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0, bottomRight[2])));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topRight[0], 0, topRight[2])));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topLeft[0], 0, topLeft[2])));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0, bottomRight[2])));
}

void Cylinder::makeFlatTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, topLeft[1], 0)));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, bottomLeft[1], 0)));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, bottomRight[1], 0)));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, topRight[1], 0)));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, topLeft[1], 0)));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(0, bottomRight[1], 0)));
}

glm::vec3 Cylinder::makeCoordinate(float theta, float height) {
    float radius = 0.5f;
    float x = radius * glm::sin(theta);
    float y = height;
    float z = radius * glm::cos(theta);
    return glm::vec3{ x, y, z };
}

glm::vec3 Cylinder::makeFlatCoordinate(float phi, float theta, float height) {
    float radius = 0.5f;
    float x = radius * glm::sin(phi) * glm::sin(theta);
    float y = height;
    float z = radius * glm::sin(phi) * glm::cos(theta);
    return glm::vec3{ x, y, z };
}

void Cylinder::makeWedge(float currentTheta, float nextTheta) {

    float currHeight = -0.5f;
    float heightIncrement = 1.f / m_param1;
    for (int i = 0; i < m_param1; i++) {

        glm::vec3 topLeft = makeCoordinate(currentTheta, currHeight + heightIncrement);
        glm::vec3 topRight = makeCoordinate(nextTheta, currHeight + heightIncrement);
        glm::vec3 bottomLeft = makeCoordinate(currentTheta, currHeight);
        glm::vec3 bottomRight = makeCoordinate(nextTheta, currHeight);

        currHeight += heightIncrement;

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cylinder::makeFlatWedges(float currentTheta, float nextTheta) {
    float phi_increment = glm::radians(90.f / m_param1);

    for (int i = 0; i < m_param1; i++) {
        float innerPhi = i * phi_increment;
        float outerPhi = (i + 1) * phi_increment;

        for (float height = -0.5f; height < 1; height++) {
            glm::vec3 topLeft = makeFlatCoordinate(outerPhi, currentTheta, height);
            glm::vec3 topRight = makeFlatCoordinate(outerPhi, nextTheta, height);
            glm::vec3 bottomLeft = makeFlatCoordinate(innerPhi, currentTheta, height);
            glm::vec3 bottomRight = makeFlatCoordinate(innerPhi, nextTheta, height);

            if (height == 0.5) {
                glm::vec3 stored = topLeft;
                topLeft = topRight;
                topRight = stored;
                stored = bottomLeft;
                bottomLeft = bottomRight;
                bottomRight = stored;
            }

            makeFlatTile(topLeft, topRight, bottomLeft, bottomRight);
        }
    }
}


void Cylinder::makeCylinder() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
        makeFlatWedges(currentTheta, nextTheta);
    }
}

void Cylinder::setVertexData() {
    // TODO for Project 5: Lights, Camera
    makeCylinder();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
