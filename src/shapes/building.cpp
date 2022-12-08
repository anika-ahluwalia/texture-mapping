#include "Building.h"
#include "Cube.h"
#include "shapes/cylinder.h"
#include <iostream>

void Building::updateParams(int param1, int size, int height, int depth, float x, float z, int texture) {
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

void Building::makeBuilding() {
    Cylinder cylinder = Cylinder();

    m_vertexData.clear();

   // for (float i = 0.f; i < 1.f + (1.f * (m_size - 1)); i++) {
        for (float j = 0.f; j < 1.f  + (1.f * (m_height - 1)); j++) {
            //for (float k = 0.f; k < 1.f + (1.f * (m_depth - 1)); k++) {
                cylinder.updateParams(m_param1, m_size, m_x, j, m_z, m_size);
                std::vector<float> cylinder_data = cylinder.getMesh();
                m_vertexData.insert(m_vertexData.end(), cylinder_data.begin(), cylinder_data.end());
            }
       // }
   // }
}

void Building::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
