#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoord;

out vec2 uvCoordinate;

void main() {
    uvCoordinate = uvCoord;
    gl_Position = vec4(position, 1.0);
}
