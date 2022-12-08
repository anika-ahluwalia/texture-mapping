#ifndef TIEREDBUILDING_H
#define TIEREDBUILDING_H

#include <vector>
#include <glm/glm.hpp>


class TieredBuilding
{
public:
    void updateParams(int param1, int size, int height, int depth, float x, float z, int texture);
    std::vector<float> getMesh() { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void makeBuilding();

    std::vector<float> m_vertexData;
    int m_param1;
    int m_size;
    int m_height;
    int m_depth;
    float m_x;
    float m_z;
    int m_texture;
};


#endif // TIEREDBUILDING_H
