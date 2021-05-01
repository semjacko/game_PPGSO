#include "cloud.h"


std::unique_ptr<ppgso::Mesh> Cloud::mesh;
std::unique_ptr<ppgso::Texture> Cloud::texture;
std::unique_ptr<ppgso::Shader> Cloud::shader;


Cloud::Cloud(glm::vec3 p, glm::vec3 r, glm::vec3 s) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("white.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cloud.obj");

    position = p;
    rotation = r;
    scale = s;
}


bool Cloud::update(Scene &scene, float dt) {
    position.z += dt;
    if (position.z >= 200) {
        position.z = -200;
    }

    generateModelMatrix();

    return true;
}


void Cloud::render(Scene &scene) {
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