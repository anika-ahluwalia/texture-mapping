#include "TieredBuilding.h"
#include "Cube.h"
#include <iostream>

void TieredBuilding::updateParams(int param1, int size, int height, int depth, float x, float z, int texture) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size;
    m_height = height;
    m_depth = depth;
    m_x = x;
    m_z = z;
    m_texture = texture;
    makeBuilding();
}

void TieredBuilding::makeBuilding() {
    Cube cube = Cube();

    m_vertexData.clear();

    float step = 1.f / m_height;

    for (float j = 0.f; j < 1.f  + (1.f * (m_height - 1)); j++) {

        float currDim = 1 - j * step;
        float currStep = currDim / 2;
        float x = m_x + currStep;
        float z = m_z + currStep;

        for (float i = x; i < currDim + (currStep * (m_size - 1)); i += currDim) {

            for (float k = z; k < currDim + (currStep * (m_depth - 1)); k += currDim) {

                cube.updateParams(m_param1, currDim, i + m_x, j, k + m_z);

                std::vector<float> cube_data = cube.generateShape();
                m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
            }
        }
    }
}

void TieredBuilding::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
