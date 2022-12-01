#version 330 core
in vec2 uvCoordinate;

uniform sampler2D texture1;

uniform bool inverting;
uniform bool blurring;

uniform bool grayscale;
uniform bool brightening;

uniform bool sharpening;
uniform bool edgeDetect;

uniform int width;
uniform int height;

out vec4 fragColor;

void main()
{
    fragColor = texture(texture1, uvCoordinate);

    // blurring filter
    if (blurring) {
        vec4 totalFrag = vec4(0);

        float widthInc = 1.f / width;
        float heightInc = 1.f / height;

        for (int i = -2; i < 3; i++) {
            float u = min(1, max(0, i * widthInc + uvCoordinate[0]));
            for (int j = -2; j < 3; j++) {
                float v = min(1, max(0,j * heightInc + uvCoordinate[1]));
                vec2 newUV = vec2(u, v);
                vec4 newFrag = texture(texture1, newUV);
                totalFrag += newFrag;
            }
        }
        fragColor = totalFrag / 25.f;
    }

    // sharpening filter -- EXTRA CREDIT
    if (sharpening) {
        vec4 totalFrag = vec4(0);

        float widthInc = 1.f / width;
        float heightInc = 1.f / height;

        for (int i = -1; i < 2; i++) {
            float u = min(1, max(0, i * widthInc + uvCoordinate[0]));
            for (int j = -1; j < 2; j++) {
                float v = min(1, max(0,j * heightInc + uvCoordinate[1]));
                vec2 newUV = vec2(u, v);
                vec4 newFrag = texture(texture1, newUV);
                if (i == 0 && j == 0) {
                    totalFrag += 17 * newFrag;
                } else {
                    totalFrag += -1 * newFrag;
                }
            }
        }
        fragColor = totalFrag / 9.f;
    }

    // edge detetction filter -- EXTRA CREDIT
    if (edgeDetect) {
        vec4 totalFrag = vec4(0);

        vec3 kernel = vec3(1, 2, 1);

        float widthInc = 1.f / width;
        float heightInc = 1.f / height;

        for (int i = -1; i < 2; i++) {
            float u = min(1, max(0, i * widthInc + uvCoordinate[0]));
            for (int j = -1; j < 2; j++) {
                float v = min(1, max(0,j * heightInc + uvCoordinate[1]));
                vec2 newUV = vec2(u, v);
                vec4 newFrag = texture(texture1, newUV);

                float weight = 0.f;
                if (i == 0) {
                    if (j == 0) {
                        weight = 4.f;
                    } else {
                        weight = -1.f;
                    }
                } else if (j == 0) {
                    weight = -1.f;
                }

                totalFrag += newFrag * weight;
            }
        }
        fragColor = totalFrag;
    }

    // inverting filter
    if (inverting) {
        fragColor[0] = 1 - fragColor[0];
        fragColor[1] = 1 - fragColor[1];
        fragColor[2] = 1 - fragColor[2];
    }

    // grayscale filter -- EXTRA CREDIT
    if (grayscale) {
        float newColor = (fragColor[0] + fragColor[1] + fragColor[2]) / 3.f;
        fragColor[0] = newColor;
        fragColor[1] = newColor;
        fragColor[2] = newColor;
    }

    // brightening filter -- EXTRA CREDIT
    if (brightening) {
        float brightFactor = 0.1;
        fragColor[0] = min(1, fragColor[0] + brightFactor);
        fragColor[1] = min(1, fragColor[1] + brightFactor);
        fragColor[2] = min(1, fragColor[2] + brightFactor);
    }
}
