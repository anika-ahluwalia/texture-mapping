#version 330 core

layout(location = 0) in vec3 objSpacePos;
layout(location = 1) in vec3 objSpaceNormal;

out vec3 worldSpacePos;
out vec3 worldSpaceNormal;
out vec4 ambient;
out vec4 diffuseCoefficients;
out vec4 specularCoefficients;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float ka;
uniform float kd;
uniform float ks;

uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;


void main() {
    worldSpacePos = vec3(modelMatrix * vec4(objSpacePos, 1));
    worldSpaceNormal = inverse(transpose(mat3(modelMatrix))) * normalize(objSpaceNormal);

    ambient = ka * cAmbient;
    diffuseCoefficients = kd * cDiffuse;
    specularCoefficients = ks * cSpecular;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(objSpacePos, 1);
}
