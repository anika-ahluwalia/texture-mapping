#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNormal;

out vec4 fragColor;

uniform int numLights;

// 0 = directional light
// 1 = point light
// 2 = spot light
uniform int lightTypes[8];

uniform vec3 lightDirections[8];
uniform vec4 lightPositions[8];
uniform vec4 lightColors[8];
uniform vec3 lightFunctions[8];

uniform float lightAngles[8];
uniform float lightPenumbras[8];

uniform vec4 ambient;
uniform vec4 diffuseCoefficients;
uniform vec4 specularCoefficients;

uniform float shininess;
uniform vec4 worldSpaceCameraPos;

void main() {

    // add ambient light
    fragColor = ambient;

    // for each of the lights in the scene
    for (int i = 0; i < numLights; ++i) {

        vec3 lighting;

        // calculating lighting and shadow vectors
        if (lightTypes[i] == 0) {
            lighting = normalize(-lightDirections[i]);
        } else {
            lighting = normalize(vec3(lightPositions[i]) - worldSpacePos);
        }

        // add diffuse light
        float diffuse_dot_product = max(min(dot(normalize(worldSpaceNormal), lighting), 1), 0);
        vec4 diffuse = lightColors[i] * diffuseCoefficients * diffuse_dot_product;

        // add specular light
        vec3 reflection = reflect(-lighting, normalize(worldSpaceNormal));
        vec3 dirToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
        float shine = min(max(dot(reflection, dirToCamera), 0), 1);
        float shine_factor;
        if (shine < 0 || (shine == 0 && shininess <= 0)) {
            shine_factor = 0;
        } else {
            shine_factor = pow(shine, shininess);
        }
        vec4 specular = lightColors[i] * specularCoefficients * shine_factor;

        // calculating attenuation
        float distance = length(worldSpacePos - vec3(lightPositions[i]));
        float att = 1 / (lightFunctions[i][0] + distance * lightFunctions[i][1] + pow(distance, 2) * lightFunctions[i][2]);
        att = min(max(att, 0), 1);
        if (lightTypes[i] != 0) {
            diffuse = att * diffuse;
            specular = att * specular;
        }

        // adjusting if it is a spot light
        if (lightTypes[i] == 2) {
            vec3 lightToPosition = normalize(worldSpacePos - vec3(lightPositions[i]));
            vec3 direction = normalize(lightDirections[i]);
            float x = acos(dot(lightToPosition, direction));
            float inner = lightAngles[i] - lightPenumbras[i];
            float falloff = -2.f * pow((x - inner) / (lightAngles[i] - inner), 3) + 3 * pow((x - inner) / (lightAngles[i] - inner), 2);

            if (x < lightAngles[i] && x > inner) {
                diffuse = (1 - falloff) * diffuse;
                specular = (1 - falloff) * specular;
            } else if (x > lightAngles[i]) {
                diffuse = vec4(0);
                specular = vec4(0);
            }
        }

        fragColor = fragColor + diffuse;
        fragColor = fragColor + specular;
    }
}
