#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNormal;

out vec4 fragColor;


uniform float ka;

uniform float kd;
uniform vec4 lightPos;

uniform float ks;
uniform float shininess;
uniform vec4 worldSpaceCameraPos;

void main() {
    // Remember that you need to renormalize vectors here if you want them to be normalized
    fragColor = vec4(0.0);

    // adding ambient component to output color
    fragColor = vec4(ka);

    // adding diffuse component to output color
    vec3 surfaceToLight = normalize(vec3(lightPos) - worldSpacePos);
    float diffuse_dot_product = dot(normalize(worldSpaceNormal), surfaceToLight);
    if (diffuse_dot_product > 1) { diffuse_dot_product = 1;}
    if (diffuse_dot_product < 0) { diffuse_dot_product = 0;}
    fragColor = fragColor + vec4(kd * diffuse_dot_product);

    // adding specular component to output color
    vec3 reflectedLight = reflect(-surfaceToLight, normalize(worldSpaceNormal));
    vec3 dirToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
    float specular_dot_product = dot(reflectedLight, dirToCamera);
    if (specular_dot_product > 1) { specular_dot_product = 1;}
    if (specular_dot_product < 0) { specular_dot_product = 0;}
    float shine = pow(specular_dot_product, shininess);
    fragColor = fragColor + vec4(ks * shine);
}
