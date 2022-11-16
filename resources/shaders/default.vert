#version 330 core

layout(location = 0) in vec3 objSpacePos;
layout(location = 1) in vec3 objSpaceNormal;

out vec3 worldSpacePos;
out vec3 worldSpaceNormal;
out vec4 ambient;
out vec4 diffuseCoefficients;
out vec4 specularCoefficients;


uniform mat4 modelMatrix;
uniform mat3 invTpModelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main() {
    worldSpacePos = vec3(modelMatrix * vec4(objSpacePos, 1));
    worldSpaceNormal = invTpModelMatrix * normalize(objSpaceNormal);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(objSpacePos, 1);
}
