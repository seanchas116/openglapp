#version 330 core

in vec2 UV;
in vec3 vertexPos_worldSpace;
in vec3 normal_cameraSpace;
in vec3 lightDirection_cameraSpace;
in vec3 eyeDirection_cameraSpace;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 lightColor;

void main() {
    vec3 n = normalize(normal_cameraSpace);
    vec3 l = normalize(lightDirection_cameraSpace);
    float cosTheta = clamp(dot(n, l), 0, 1);
    vec3 diffuseColor = texture(textureSampler, UV).rgb;
    //color = diffuseColor * lightColor * cosTheta;
    color = diffuseColor * lightColor * cosTheta;
}
