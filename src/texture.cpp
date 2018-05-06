#include <GL/glew.h>
#include <gli/gli.hpp>

GLuint loadImage(const char* imagePath) {
    gli::texture Texture = gli::load(imagePath);
    if(Texture.empty())
        return 0;
    Texture = gli::flip(Texture);

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
    GLenum Target = GL.translate(Texture.target());
    assert(gli::is_compressed(Texture.format()));

    GLuint TextureName = 0;
    glGenTextures(1, &TextureName);
    glBindTexture(Target, TextureName);
    glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
    glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
    glm::tvec3<GLsizei> Extent(Texture.extent(0));
    glTexStorage2D(Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x, Extent.y);
    for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
    {
        glm::tvec3<GLsizei> Extent(Texture.extent(Level));
        glCompressedTexSubImage2D(
            Target, static_cast<GLint>(Level), 0, 0, Extent.x, Extent.y,
            Format.Internal, static_cast<GLsizei>(Texture.size(Level)), Texture.data(0, 0, Level));
    }

    return TextureName;
}
