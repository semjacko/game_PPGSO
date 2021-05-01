#include "animal.h"


std::unique_ptr<ppgso::Mesh> Animal::mesh;
std::unique_ptr<ppgso::Texture> Animal::texture;
std::unique_ptr<ppgso::Shader> Animal::shader;


Animal::Animal(glm::vec3 p, glm::vec3 r, glm::vec3 s, Animator animator) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("penguin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("penguin.obj");

    position = p;
    rotation = r;
    scale = s;

    collisionPoints[0] = {0.3, 1.3, 0.4};
    collisionPoints[1] = {-0.3, -0.1, -0.6};
    updateCollider();

    this->animator = animator;
}


bool Animal::update(Scene &scene, float dt) {
    if (animate) {
        Keyframe keyframe = animator.getCurrentKeyframe(dt);
        position = keyframe.position;
        updateCollider();
        rotation = keyframe.rotation;
        scale = keyframe.scale;
    }

    generateModelMatrix();

    return true;
}


void Animal::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera.projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera.viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("LightPosition", scene.lightPosition);
    shader->setUniform("LightColor", scene.lightColor);
    shader->setUniform("CameraPosition", scene.camera.position);
    shader->setUniform("IsLamp", scene.getId() == 1);

    mesh->render();
}