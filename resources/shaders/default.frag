#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNormal;
in vec4 ambient;
in vec4 diffuseCoefficients;
in vec4 specularCoefficients;

out vec4 fragColor;

uniform vec3 lightDirections[8];
uniform vec4 lightColors[8];

uniform float shininess;
uniform vec4 worldSpaceCameraPos;

void main() {

    // Remember that you need to renormalize vectors here if you want them to be normalized
    fragColor = ambient;

    for (int i = 0; i < 8; ++i) {
        vec3 lighting = normalize(-lightDirections[i]);
        float diffuse_dot_product = max(min(dot(worldSpaceNormal, lighting), 1), 0);
        vec4 diffuse = lightColors[i] * diffuseCoefficients * diffuse_dot_product;
        fragColor = fragColor + diffuse;

        vec3 reflection = reflect(-lighting, worldSpaceNormal);
        vec3 dirToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
        // check 0 cases
        float shine = max(glm::dot(reflection, dirToCamera), 0);
        vec4 specular = lightColors[i] * specularCoefficients * pow(shine, shininess);
        fragColor = fragColor + specular
    }
}
