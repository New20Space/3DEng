
#include <string>
#include <GL/glew.h>

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    // Should be called once to load the texture
    bool Load();

    // Must be called at least once for the specific texture unit
    void Bind(GLenum TextureUnit);

private:

    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_imageBPP = 0;
};
