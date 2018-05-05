#pragma once
#include <GL/glew.h>
#include <vector>

class Model {
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint normalBufferID;
    GLuint uvBufferID;
    GLuint indexBufferID;
    size_t count;

public:
    Model(const char* objPath);

    void draw();
};
