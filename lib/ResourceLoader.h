#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

class ResourceLoader {
public:
    ResourceLoader();
    ~ResourceLoader();

    static GLuint createShaderProgram(const char *vertex_file_path, const char *fragment_file_path);

private:
    static GLuint createShader(GLenum shaderType, const char *filepath);
};

#endif // SHADER_H
