#include "camera.h"

Camera::Camera(Program* program)
    : eye(vec3(0.0f)), at(vec3(0.0f)), up(vec3(0.0f, 1.0f, 0.0f)),
    fovy(60.0f), aspect(1.0f), zNear(1.0f), zFar(400.0f) {

    this->create(program);
}

void Camera::create(Program* program) {
    LOG_PRINT_DEBUG("Create camera");

    this->program = program;

    if(glGetUniformLocation(program->get(), "viewMat") < 0)
        LOG_PRINT_WARN("Fail to get uniform location: %s", "viewMat");
    if(glGetUniformLocation(program->get(), "projMat") < 0)
        LOG_PRINT_WARN("Fail to get uniform location: %s", "projMat");
}

void Camera::update() const {
    mat4 viewMat = lookAt(eye, at, up);
    mat4 projMat = perspective(radians(fovy), aspect, zNear, zFar);

    GLint viewMatLoc = glGetUniformLocation(program->get(), "viewMat");
    GLint projMatLoc = glGetUniformLocation(program->get(), "projMat");

    if (viewMatLoc >= 0) glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, value_ptr(viewMat));
    else LOG_PRINT_ERROR("Fail to get uniform location: %s", "viewMat");
    if (projMatLoc >= 0) glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
    else LOG_PRINT_ERROR("Fail to get uniform location: %s", "projMat");
}