#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "shader.hpp"
#include "controls.hpp"
#include "texture.hpp"
#include "model.hpp"

using namespace glm;

GLFWwindow* initialize() {
    GLFWwindow* window;
    glewExperimental = true;
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Playground", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    return window;
}

int main() {
    auto window = initialize();
    if (!window) {
        return -1;
    }
    Controls controls(window);

    glClearColor(0.9, 0.9, 0.9, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // shader

    GLuint programID = LoadShaders("./resources/shader.vert", "./resources/shader.frag");
    glUseProgram(programID);

    GLuint textureID = glGetUniformLocation(programID, "textureSampler");
    GLuint texture = loadImageGLI("./resources/uvmap.DDS");
    glUniform1i(textureID, 0);

    Model model("./resources/suzanne.obj");

    do {
        controls.computeFromInputs();

        mat4 modelMatrix(1);
        mat4 mvpMatrix = controls.getProjectionMatrix() * controls.getViewMatrix() * modelMatrix;

        GLuint matrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
