#ifndef HW3_MATERIAL_H
#define HW3_MATERIAL_H

#include "global.h"
#include "program.h"
#include "texture.h"

class Material {
public:
    vec3 specular, ambient, emissive;
    float shininess;
    float threshold;
    float displacement;

    Material(Program* program, Texture* textureDiff=nullptr);
    virtual ~Material();

    virtual void update() const;

private:
    Program* program;
    Texture* textureDiff;
    Texture* textureDissolve;
    virtual void create(Program* program, Texture* textureDiff=nullptr);
};

#endif //HW3_MATERIAL_H
