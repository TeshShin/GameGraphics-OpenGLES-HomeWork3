#include "shader.h"

Shader::Shader(GLenum type, const char* fileName) {
    create(type);
    source(fileName);
    compile();
}

Shader::~Shader() {
    destroy();
}

void Shader::create(GLenum type) {
    if (type != GL_VERTEX_SHADER and type != GL_FRAGMENT_SHADER) {
        LOG_PRINT_ERROR("Wrong shader type assigned: %d", type);
        return;
    }

    LOG_PRINT_DEBUG("Create %s shader", type == GL_VERTEX_SHADER ? "vertex" : "fragment");

    this->shader = glCreateShader(type);
    if(this->shader == 0) {
        LOG_PRINT_ERROR("Fail to create shader");
    }
}

void Shader::source(const char* fileName) const {
    LOG_PRINT_DEBUG("Set shader source");

    const char* src = Asset::load(fileName);
    if (src == nullptr) {
        LOG_PRINT_ERROR("Fail to load shader source");
        return;
    }
    LOG_PRINT_DEBUG("Shader source: \n%s", src);
    glShaderSource(this->shader, 1, &src, NULL);
    delete[] src;
}

void Shader::compile() const {
    LOG_PRINT_DEBUG("Compile shader");

    glCompileShader(this->shader);

    GLint compileStatus;
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == 0) {
        GLint infoLogLength;
        glGetShaderiv(this->shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength) {
            GLchar* const infoLog = new GLchar[infoLogLength];
            glGetShaderInfoLog(this->shader, infoLogLength, NULL, infoLog);
            LOG_PRINT_ERROR("Fail to compile shader: \n%s", infoLog);
            delete[] infoLog;
        }
    }
}

void Shader::destroy() const {
    LOG_PRINT_DEBUG("Delete shader");

    glDeleteShader(this->shader);
}