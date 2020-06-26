#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"
#include "binary/red.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;
Object* Scene::plane = nullptr;
Texture* Scene::planeColor = nullptr;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    //player
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));


    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
    Scene::lineDraw->worldMat = scale(vec3(1.0f / 3.0f));

    // plain
    Scene::planeColor = new Texture(Scene::program, 0, "textureDiff", redTexels, redSize);
    Scene::plane = new Object(program, material, planeVertices, planeIndices);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();

    Scene::camera->update();

    const float playTime = 5.0;
    static float time = 0.0f;
    time += deltaTime;
    if (time >= playTime) time -= playTime;

    int prevFrame = (int)floor(time / playTime * motions.size());
    int nextFrame = (int)ceil(time / playTime * motions.size());
    if(nextFrame >= motions.size()) nextFrame = 0;
    float ratio = time / playTime * motions.size() - (float)prevFrame;

    // from bone space to world space
    vector<mat4x4> bone2world = {mat4x4(1.0f)};
    // from world space to bone space
    vector<mat4x4> world2bone = {mat4x4(1.0f)};
    vector<Vertex> skeletonPos = {{jOffset[0]}};
    vector<Index> skeletonIndex;
    for(int jointIdx = 1; jointIdx < jNames.size(); jointIdx ++) {
        //Motion interpolation using an Euler angle or quaternion.
        // Please use prevFrame, nextFrame, ratio, R, bone2world, and world2bone.
        int parentIdx = jParents[jointIdx];
        float degreeX, degreeY, degreeZ;
        if(nextFrame == 0){
            degreeX = motions[prevFrame][3*jointIdx + 3];
            degreeY = motions[prevFrame][3*jointIdx + 4];
            degreeZ = motions[prevFrame][3*jointIdx + 5];

        }
        else{
            degreeX = glm::mix(motions[prevFrame][3*jointIdx + 3],motions[nextFrame][3*jointIdx + 3], ratio);
            degreeY = glm::mix(motions[prevFrame][3*jointIdx + 4],motions[nextFrame][3*jointIdx + 4], ratio);
            degreeZ = glm::mix(motions[prevFrame][3*jointIdx + 5],motions[nextFrame][3*jointIdx + 5], ratio);
        }
        mat4x4 Rx = {
                1.0f,0.0f,0.0f,0.0f,
                0.0f,cos(radians(degreeX)),sin(radians(degreeX)),0.0f,
                0.0f,-sin(radians(degreeX)),cos(radians(degreeX)),0.0f,
                0.0f,0.0f,0.0f,1.0f
        };
        mat4x4 Ry = {
                cos(radians(degreeY)), 0.0f, -sin(radians(degreeY)), 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin(radians(degreeY)), 0.0f, cos(radians(degreeY)), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        mat4x4 Rz = {
                cos(radians(degreeZ)),sin(radians(degreeZ)), 0.0f,0.0f,
                -sin(radians(degreeZ)),cos(radians(degreeZ)), 0.0f,0.0f,
                0.0f,0.0f,1.0f,0.0f,
                0.0f,0.0f,0.0f,1.0f
        };


        float x = jOffset[jointIdx][0];
        float y = jOffset[jointIdx][1];
        float z = jOffset[jointIdx][2];
        mat4x4 R = Rz * Rx * Ry;
        mat4x4 T = {
                1.0f,0.0f,0.0f,0.0f,
                0.0f,1.0f,0.0f,0.0f,
                0.0f,0.0f,1.0f,0.0f,
                x,   y,   z,   1.0f
        };
        mat4x4 inverseT = {
                1.0f,0.0f,0.0f,0.0f,
                0.0f,1.0f,0.0f,0.0f,
                0.0f,0.0f,1.0f,0.0f,
                -x,  -y,  -z,  1.0f
        };
        // With motion
        bone2world.push_back(bone2world[parentIdx]* T * R);
        world2bone.push_back(inverseT * world2bone[parentIdx]);
        skeletonPos.push_back({vec3(column(bone2world[jointIdx],3))});
        skeletonIndex.push_back(jointIdx);
        skeletonIndex.push_back(parentIdx);
    }

    //set new vertex
    vector<Vertex> newVertex;
    float vertexX, vertexY, vertexZ;
    vec4 vertexPos;

    for(int vertexIdx = 0; vertexIdx <= playerVertices.size(); vertexIdx++){
        vec3 result;
        vertexX = playerVertices[vertexIdx].pos[0];
        vertexY = playerVertices[vertexIdx].pos[1];
        vertexZ = playerVertices[vertexIdx].pos[2];

        vertexPos = {vertexX, vertexY, vertexZ,1.0f};
        for(int boneIter = 0; boneIter < 4; boneIter ++) {
            int boneIndex = playerVertices[vertexIdx].bone[boneIter];
            if (boneIndex != -1) {
                result += vec3(playerVertices[vertexIdx].weight[boneIter] * bone2world[boneIndex]* world2bone[boneIndex] * vertexPos);

            }
        }
        newVertex.push_back( {result, playerVertices[vertexIdx].nor, playerVertices[vertexIdx].tex, playerVertices[vertexIdx].bone, playerVertices[vertexIdx].weight} );

    }
    Scene::player->load(newVertex, playerIndices);
    Scene::player->draw();

    //If you want to see the skeleton as a line..
    //glLineWidth(20);
    //Scene::lineDraw->load(skeletonPos, skeletonIndex);
    //Scene::lineDraw->draw();
    Scene::plane->draw();

}