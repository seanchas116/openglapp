#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shader.hpp"

using namespace glm;

static const ivec2 windowSize = {1024, 768};

static GLFWwindow* window;

struct Controls {
    vec3 position = vec3(0, 0, 5);
    // zx angle
    float horizontalAngle = M_PI;
    // xy angle
    float verticalAngle = 0;

    float initialFoV = 45;

    float speed = 3;
    float mouseSpeed = 0.005;

    mat4 projectionMatrix;
    mat4 viewMatrix;

    void computeFromInputs() {
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwSetCursorPos(window, windowSize.x / 2, windowSize.y / 2);
        float xdiff = windowSize.x / 2 - xpos;
        float ydiff = windowSize.y / 2 - ypos;
        std::cout << xdiff << "," << ydiff << std::endl;

        horizontalAngle += mouseSpeed * float(xdiff);
        verticalAngle += mouseSpeed * float(ydiff);
        std::cout << horizontalAngle << "," << verticalAngle << std::endl;

        vec3 direction = vec3(cos(verticalAngle) * sin(horizontalAngle),
                              sin(verticalAngle),
                              cos(verticalAngle) * cos(horizontalAngle));
        vec3 right = vec3(sin(horizontalAngle - M_PI/2.f),
                          0,
                          cos(horizontalAngle - M_PI/2.f));
        //vec3 up = cross(right, direction);
        vec3 up = vec3(0, 1, 0);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            position += direction * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            position -= direction * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            position += right * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            position -= right * deltaTime * speed;
        }

        projectionMatrix = glm::perspective(glm::radians(initialFoV), float(windowSize.x) / float(windowSize.y), 0.1f, 10000.f);
        viewMatrix = glm::lookAt(position, position + direction, up);
    }
};

Controls controls;

static const GLfloat vertexData[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static const GLfloat uvData[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};

bool initialize() {
    glewExperimental = true;
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowSize.x, windowSize.y, "Playground", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

int main() {
    if (!initialize()) {
        return -1;
    }

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // vertex array

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // vertex buffer

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvData), uvData, GL_STATIC_DRAW);

    // shader

    GLuint programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");
    glUseProgram(programID);

    GLuint textureID = glGetUniformLocation(programID, "textureSampler");
    GLuint texture = loadBMP_custom("./uvtemplate.bmp");
    glUniform1i(textureID, 0);

    do {
        controls.computeFromInputs();

        mat4 modelMatrix;
        mat4 mvpMatrix = controls.projectionMatrix * controls.viewMatrix * modelMatrix;

        GLuint matrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}
