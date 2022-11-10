#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    // Task 4: populate renderData with global data, lights, and camera data
    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();
    renderData.cameraData = fileReader.getCameraData();

    // Task 5: populate renderData's list of primitives and their transforms.
    //         This will involve traversing the scene graph, and we recommend you
    //         create a helper function to do so!
    SceneNode* root = fileReader.getRootNode();
    renderData.shapes.clear();
    glm::mat4 m4( 1.0f );
    renderData.shapes = dfs(root, m4);

    return true;
}

std::vector<RenderShapeData> SceneParser::dfs(SceneNode* root, glm::mat4 prev_transforms, std::vector<RenderShapeData> shapes) {
    if (root->transformations.size() > 0) {
        glm::mat4 curr_transforms = make_matrix(root->transformations);
        prev_transforms = prev_transforms * curr_transforms;
    }

    if (root->primitives.size() > 0) {
        for (ScenePrimitive* primitive: root->primitives) {
            RenderShapeData data = {*primitive, prev_transforms};
            shapes.push_back(data);
        }
    } else {
        if (root->children.size() > 0) {
            for (SceneNode* child: root->children) {
                shapes = dfs(child, prev_transforms, shapes);
            }
        }
    }

    return shapes;
}


glm::mat4 SceneParser::make_matrix(std::vector<SceneTransformation*> transforms) {
    glm::mat4 matrix ( 1.0f );

    for (SceneTransformation* t: transforms) {

        glm::mat4 this_matrix;

        switch (t->type) {
            case TransformationType::TRANSFORMATION_TRANSLATE:
                this_matrix = glm::translate(t->translate);
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                this_matrix = glm::rotate(t->angle, t->rotate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                this_matrix = glm::scale(t->scale);
                break;
            case TransformationType::TRANSFORMATION_MATRIX:
                this_matrix = t->matrix;
                break;
        }

        matrix = matrix * this_matrix;
    }

    return matrix;
}

