#version 330 core

layout(location = 0) in vec3 objSpacePos;

out vec3 worldSpacePos;
out vec3 worldSpaceNormal;


uniform mat4 modelMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    worldSpacePos = vec3(modelMatrix * vec4(objSpacePos, 1));
    worldSpaceNormal = inverse(transpose(mat3(modelMatrix))) * normalize(objSpacePos);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(objSpacePos, 1);
}
