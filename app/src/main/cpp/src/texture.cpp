#include "texture.h"

Texture::Texture(Program *program, const int number, const string name,
        const vector<Texel> &data, const GLsizei size) {

    create(program, number, name);
    load(data, size);
}

Texture::~Texture() {
    destroy();
}

void Texture::create(Program* program, const int number, const string name) {
    LOG_PRINT_DEBUG("Create texture: %s", name.c_str());

    this->program = program;
    this->number = number;
    this->name = name;

    glGenTextures(1, &id);
    if (id == 0)
        LOG_PRINT_ERROR("Fail to generate texture: %s", this->name.c_str());

    if (glGetUniformLocation(program->get(), this->name.c_str()) < 0)
        LOG_PRINT_WARN("Fail to get uniform location: %s", this->name.c_str());
}

void Texture::load(const vector<Texel> &data, const GLsizei size) {
    LOG_PRINT_DEBUG("Load texture data");

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size,
            0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::update() const {
    GLint textureLoc = glGetUniformLocation(program->get(), this->name.c_str());

    glActiveTexture((GLenum)(GL_TEXTURE0 + number));
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (textureLoc >= 0) glUniform1i(textureLoc, number);
    else LOG_PRINT_ERROR("Fail to get uniform location: %s", this->name.c_str());
}

void Texture::destroy() {
    LOG_PRINT_DEBUG("Destroy texture");
    glDeleteTextures(1, &id);
}