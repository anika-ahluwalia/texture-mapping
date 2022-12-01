#version 330 core
in vec2 uvCoordinate;

uniform sampler2D texture1;
uniform bool inverting;
uniform bool blurring;

uniform int width;
uniform int height;

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

        vec4 totalFrag = vec4(0);

        float widthInc = 1.f / width;
        float heightInc = 1.f / height;

        for (int i = -2; i < 3; i++) {
            float u = i * widthInc + uvCoordinate[0];
            for (int j = -2; j < 3; j++) {
                float v = j * heightInc + uvCoordinate[1];
                vec2 newUV = vec2(u, v);
                vec4 newFrag = texture(texture1, newUV);
                totalFrag += newFrag;
            }
        }

        fragColor = totalFrag / 25.f;
    }
}
