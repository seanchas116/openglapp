#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Controls {
    GLFWwindow* window;

    glm::vec3 position = glm::vec3(0, 0, 5);
    // zx angle
    float horizontalAngle = M_PI;
    // xy angle
    float verticalAngle = 0;

    float initialFoV = 45;

    float speed = 3;
    float mouseSpeed = 0.005;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

public:
    Controls(GLFWwindow* window) : window(window) {}

    void computeFromInputs();
    glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    glm::mat4 getViewMatrix() const { return viewMatrix; }
};
