#include "simpleObject.h"


std::unique_ptr<ppgso::Mesh> SimpleObject::mesh[OBJ_COUNT];
std::unique_ptr<ppgso::Texture> SimpleObject::texture[OBJ_COUNT];
std::unique_ptr<ppgso::Shader> SimpleObject::shader;
char *SimpleObject::textureName[OBJ_COUNT] = {"wall.bmp", "ground.bmp", "floor.bmp", "ceiling.bmp", "grass.bmp", "grass.bmp", "cliff.bmp", "gray.bmp", "gray.bmp", "door.bmp", "yellow.bmp", "white.bmp"};
char *SimpleObject::meshName[OBJ_COUNT] = {"cube.obj", "cube.obj", "cube.obj", "cube.obj", "tree.obj", "grass.obj", "cliff.obj", "lamp.obj", "bed.obj", "door.obj", "sphere.obj", "cloud.obj"};


SimpleObject::SimpleObject(glm::vec3 p, glm::vec3 r, glm::vec3 s, uint8_t id) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture[id]) texture[id] = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureName[id]));
    if (!mesh[id]) mesh[id] = std::make_unique<ppgso::Mesh>(meshName[id]);

    position = p;
    rotation = r;
    scale = s;

    collisionPoints[0] = collisionPoints[1] = {0,0,0};

    if (id <= 3) { // kocky
        collisionPoints[0] = {-.5, -.5, -.5};
        collisionPoints[1] = {.5, .5, .5};
    }
    if (id == 4) { // strom
        collisionPoints[0] = {.8, 3, .8};
        collisionPoints[1] = {-.8, -2, -0.8};
    }
    else if (id == 6) { // utes
        collisionPoints[0] = {-7, 8, 13};
        collisionPoints[1] = {1, -1, -10};
    }
    updateCollider();

    this->id = id;
}


bool SimpleObject::update(Scene &scene, float dTime) {
    generateModelMatrix();

    return true;
}


void SimpleObject::render(Scene &scene) {
    if (id == 6) {  // kvoli jaskyni
        glDisable(GL_CULL_FACE);
    }

    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera.projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera.viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture[id]);
    shader->setUniform("LightPosition", scene.lightPosition);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("CameraPosition", scene.camera.position);
    shader->setUniform("IsLamp", scene.getId() == 1);

    mesh[id]->render();

    if (id == 6) {  // kvoli jaskyni
        glEnable(GL_CULL_FACE);
    }
}
