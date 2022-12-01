#version 330 core
in vec2 uvCoordinate;

uniform sampler2D texture1;
uniform bool inverting;
uniform bool blurring;

out vec4 fragColor;

void main()
{
    fragColor = texture(texture1, uvCoordinate);

    if (inverting) {
        fragColor[0] = 1 - fragColor[0];
        fragColor[1] = 1 - fragColor[1];
        fragColor[2] = 1 - fragColor[2];
    }

    if (blurring) {

    }

}
