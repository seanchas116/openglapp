#include "controls.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

void Controls::computeFromInputs() {
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;

    double xpos, ypos;
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    float xdiff = windowWidth / 2 - xpos;
    float ydiff = windowHeight / 2 - ypos;

    horizontalAngle += mouseSpeed * float(xdiff);
    verticalAngle -= mouseSpeed * float(ydiff);

    vec3 direction = vec3(cos(verticalAngle) * sin(horizontalAngle),
                          sin(verticalAngle),
                          cos(verticalAngle) * cos(horizontalAngle));
    vec3 right = vec3(sin(horizontalAngle - M_PI/2.f),
                      0,
                      cos(horizontalAngle - M_PI/2.f));
    //vec3 up = cross(right, direction);
    vec3 up = vec3(0, 1, 0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }

    projectionMatrix = glm::perspective(glm::radians(initialFoV), float(windowWidth) / float(windowHeight), 0.1f, 10000.f);
    viewMatrix = glm::lookAt(position, position + direction, up);
}
