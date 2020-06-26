#include "program.h"

Program::Program(Shader* vertexShader, Shader* fragmentShader) {
    create();
    attach(vertexShader, fragmentShader);
    link();
    use();
}

Program::~Program() {
    destroy();
}

void Program::create() {
    LOG_PRINT_DEBUG("Create program");

    this->program = glCreateProgram();
    if(program == 0) {
        LOG_PRINT_ERROR("Fail to create program");
    }
}

void Program::attach(Shader* vertexShader, Shader* fragmentShader) {
    LOG_PRINT_DEBUG("Attach shaders to program");

    glAttachShader(this->program, vertexShader->get());
    glAttachShader(this->program, fragmentShader->get());
}

void Program::link() const {
    LOG_PRINT_DEBUG("Link program");

    glLinkProgram(this->program);

    GLint linkStatus;
    glGetProgramiv(this->program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == 0) {
        GLint infoLogLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if(infoLogLength) {
            GLchar* const infoLog = new GLchar[infoLogLength];
            glGetProgramInfoLog(this->program, infoLogLength, NULL, infoLog);
            LOG_PRINT_ERROR("Fail to link program: \n%s", infoLog);
            delete[] infoLog;
        }
    }
}

void Program::use() const {
    glUseProgram(this->program);
}

void Program::destroy() {
    LOG_PRINT_DEBUG("Delete program");

    glDeleteProgram(this->program);
}