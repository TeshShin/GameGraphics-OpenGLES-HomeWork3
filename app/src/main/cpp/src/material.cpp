#include "material.h"

Material::Material(Program* program, Texture* textureDiff){
    create(program, textureDiff);
}

Material::~Material() {

}

void Material::create(Program* program, Texture* textureDiff) {
    LOG_PRINT_DEBUG("Create material");

    this->program = program;
    this->textureDiff = textureDiff;
}

void Material::update() const {
    if (textureDiff) textureDiff->update();
}