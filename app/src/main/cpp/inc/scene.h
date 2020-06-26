#ifndef HW3_SCENE_H
#define HW3_SCENE_H

#include "global.h"
#include "program.h"
#include "camera.h"
#include "object.h"
#include "texture.h"
#include "material.h"

class Scene {
public:
    static void setup(AAssetManager* aAssetManager);
    static void screen(int width, int height);
    static void update(float deltaTime);

private:
    static Shader* vertexShader;
    static Shader* fragmentShader;
    static Program* program;
    static Camera* camera;
    static Texture* diffuse;
    static Material* material;
    static Object* player;

    static Object* plane;
    static Texture* planeColor;

    static Object* lineDraw;
    static Texture* lineColor;
    static Material* lineMaterial;
};

#endif //HW3_SCENE_H
