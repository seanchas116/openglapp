#version 330 core

layout(location = 0) in vec3 vertexPos_modelSpace;
layout(location = 1) in vec3 vertexNormal_modelSpace;
layout(location = 2) in vec2 vertexUV;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 lightPos_worldSpace;

out vec2 UV;
out vec3 vertexPos_worldSpace;
out vec3 normal_cameraSpace;
out vec3 lightDirection_cameraSpace;
out vec3 eyeDirection_cameraSpace;

void main() {
    gl_Position = MVP * vec4(vertexPos_modelSpace, 1);

    vertexPos_worldSpace = (M * vec4(vertexPos_modelSpace, 1)).xyz;

    vec3 vertexPos_cameraSpace = (V * vec4(vertexPos_worldSpace, 1)).xyz;
    eyeDirection_cameraSpace = vec3(0) - vertexPos_cameraSpace;

    vec3 lightPos_cameraSpace = (V * vec4(lightPos_worldSpace, 1)).xyz;
    lightDirection_cameraSpace = lightPos_cameraSpace - vertexPos_cameraSpace;

    normal_cameraSpace = (V * M * vec4(vertexNormal_modelSpace, 0)).xyz;

    UV = vertexUV;
}
