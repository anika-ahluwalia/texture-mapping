#include "sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));
}

glm::vec3 Sphere::makeCoordinate(float phi, float theta) {
    float radius = 0.5f;
    float x = glm::sin(phi) * glm::sin(theta);
    float y = glm::cos(phi);
    float z = glm::sin(phi) * glm::cos(theta);
    return radius * glm::vec3{ x, y, z };
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float phi_increment = glm::radians(180.f / m_param1);

    for (int i = 0; i < m_param1; i++) {
        float topPhi = i * phi_increment;
        float bottomPhi = (i + 1) * phi_increment;

        glm::vec3 topLeft = makeCoordinate(topPhi, currentTheta);
        glm::vec3 topRight = makeCoordinate(topPhi, nextTheta);
        glm::vec3 bottomLeft = makeCoordinate(bottomPhi, currentTheta);
        glm::vec3 bottomRight = makeCoordinate(bottomPhi, nextTheta);

        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
     makeSphere();
}

void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
