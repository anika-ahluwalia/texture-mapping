#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNormal;
in vec4 ambient;
in vec4 diffuseCoefficients;
in vec4 specularCoefficients;

out vec4 fragColor;

uniform int numLights;

uniform vec3 lightDirections[8];
uniform vec4 lightColors[8];

uniform float shininess;
uniform vec4 worldSpaceCameraPos;

void main() {

    fragColor = ambient;

    for (int i = 0; i < numLights; ++i) {
        vec3 lighting = normalize(-lightDirections[i]);
        float diffuse_dot_product = max(min(dot(normalize(worldSpaceNormal), lighting), 1), 0);
        vec4 diffuse = lightColors[i] * diffuseCoefficients * diffuse_dot_product;
        fragColor = fragColor + diffuse;

        vec3 reflection = reflect(-lighting, normalize(worldSpaceNormal));
        vec3 dirToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
        float shine = min(max(dot(reflection, dirToCamera), 0), 1);
        float shine_factor;
        if (shine < 0 || (shine == 0 & shininess <= 0)) {
            shine_factor = 0;
        } else {
            shine_factor = pow(shine, shininess);
        }
        vec4 specular = lightColors[i] * specularCoefficients * shine_factor;
        fragColor = fragColor + specular;
    }
}
