#include "texture.hpp"
#include <GL/glew.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint loadTexture(const char* path) {
    int width, height, nChannels;
    uint8_t *data = stbi_load(path, &width, &height, &nChannels, 3);
    int stride = width * 3;
    std::vector<uint8_t> flippedData(width * height * nChannels);

    // flip y
    for (int y = 0; y < height; ++y) {
        auto src = data + y * stride;
        auto dst = flippedData.data() + (height - y - 1) * stride;
        memcpy(dst, src, stride);
    }
    stbi_image_free(data);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, flippedData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}
