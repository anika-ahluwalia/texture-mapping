#include "cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Cone::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topLeft[0], 0.5f*sqrt(pow(topLeft[0], 2) + pow(topLeft[2], 2)), topLeft[2])));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomLeft[0], 0.5f*sqrt(pow(bottomLeft[0], 2) + pow(bottomLeft[2], 2)), bottomLeft[2])));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0.5f*sqrt(pow(bottomRight[0], 2) + pow(bottomRight[2], 2)), bottomRight[2])));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topRight[0], 0.5f*sqrt(pow(topRight[0], 2) + pow(topRight[2], 2)), topRight[2])));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(topLeft[0], 0.5f*sqrt(pow(topLeft[0], 2) + pow(topLeft[2], 2)), topLeft[2])));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0.5f*sqrt(pow(bottomRight[0], 2) + pow(bottomRight[2], 2)), bottomRight[2])));
}

void Cone::makeTopTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    glm::vec3 avgTop = (bottomLeft + bottomRight) / 2.f;
    float topX = 0.5f * (bottomLeft[0] + bottomRight[0]);
    float topZ = 0.5f * (bottomLeft[2] + bottomRight[2]);
    float topY = 0.5f * sqrt(pow(topX, 2) + pow(topZ, 2));
    glm::vec3 topNormal = glm::normalize(glm::vec3(topX, topY, topZ));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topNormal);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomLeft[0], 0.5f*sqrt(pow(bottomLeft[0], 2) + pow(bottomLeft[2], 2)), bottomLeft[2])));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0.5f*sqrt(pow(bottomRight[0], 2) + pow(bottomRight[2], 2)), bottomRight[2])));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topNormal);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topNormal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::vec3(bottomRight[0], 0.5f*sqrt(pow(bottomRight[0], 2) + pow(bottomRight[2], 2)), bottomRight[2])));
}

void Cone::makeFlatTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
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

glm::vec3 Cone::makeCoordinate(float theta, float radius, float height) {
    float x = radius * glm::sin(theta);
    float y = height;
    float z = radius * glm::cos(theta);
    return glm::vec3{ x, y, z };
}

glm::vec3 Cone::makeFlatCoordinate(float phi, float theta, float height) {
    float radius = 0.5f;
    float x = radius * glm::sin(phi) * glm::sin(theta);
    float y = height;
    float z = radius * glm::sin(phi) * glm::cos(theta);
    return glm::vec3{ x, y, z };
}

void Cone::makeWedge(float currentTheta, float nextTheta) {

    float currHeight = -0.5f;
    float heightIncrement = 1.f / m_param1;

    float currRadius = 0.5f;
    float radiusIncrement = 0.5f / m_param1;

    float episilon = 0.01f;

    for (int i = 0; i < m_param1; i++) {

        glm::vec3 topLeft = makeCoordinate(currentTheta, currRadius - radiusIncrement, currHeight + heightIncrement);
        glm::vec3 topRight = makeCoordinate(nextTheta, currRadius - radiusIncrement, currHeight + heightIncrement);
        glm::vec3 bottomLeft = makeCoordinate(currentTheta, currRadius, currHeight);
        glm::vec3 bottomRight = makeCoordinate(nextTheta, currRadius, currHeight);

        currHeight += heightIncrement;
        currRadius -= radiusIncrement;

        if (abs(0.5f - currHeight) > episilon) {
            makeTile(topLeft, topRight, bottomLeft, bottomRight);
        } else {
            makeTopTile(topLeft, topRight, bottomLeft, bottomRight);
        }
    }
}

void Cone::makeFlatWedges(float currentTheta, float nextTheta) {
    float phi_increment = glm::radians(90.f / m_param1);

    for (int i = 0; i < m_param1; i++) {
        float innerPhi = i * phi_increment;
        float outerPhi = (i + 1) * phi_increment;

        glm::vec3 topLeft = makeFlatCoordinate(outerPhi, currentTheta, -0.5f);
        glm::vec3 topRight = makeFlatCoordinate(outerPhi, nextTheta, -0.5f);
        glm::vec3 bottomLeft = makeFlatCoordinate(innerPhi, currentTheta, -0.5f);
        glm::vec3 bottomRight = makeFlatCoordinate(innerPhi, nextTheta, -0.5f);


        makeFlatTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}


void Cone::makeCone() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = (i + 1) * thetaStep;
        makeWedge(currentTheta, nextTheta);
        makeFlatWedges(currentTheta, nextTheta);
    }
}

void Cone::setVertexData() {
    makeCone();
}

void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
