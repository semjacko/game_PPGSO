#include "popup.h"


std::unique_ptr<ppgso::Mesh> Popup::mesh;
std::unique_ptr<ppgso::Texture> Popup::texture[POPUP_COUNT];
std::unique_ptr<ppgso::Shader> Popup::shader;
char *Popup::textureName[POPUP_COUNT] = {"popup1.bmp", "popup2.bmp"};


Popup::Popup(int id) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture[id]) texture[id] = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureName[id]));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");

    scale = {1,0.6,1};

    this->id = id;
}


bool Popup::update(Scene &scene, float dt) {
    if (id == 0 && scene.player.position.z > -2) {
        return false;
    }
    if (id == 1 && (scene.player.position.x > -7 || scene.player.position.z > -37)) {
        return false;
    }

    position = scene.player.position + -glm::normalize(scene.camera.back) * 3.f;
    position.y += 2.5;

    if (id == 0) {
        position.y = position.y < 1.5f ? 1.5f : position.y;
        position.y = position.y > 5.4f ? 5.4f : position.y;
        position.x = position.x > 4.f ? 4.f : position.x;
        position.x = position.x < -4.f ? -4.f : position.x;
        position.z = position.z < -7.f ? -7.f : position.z;
        rotation.y = -scene.cursor.yaw - 90.f;
        rotation.x = scene.cursor.pitch;
    }
    else if (id == 1) {
        position.y = position.y < 1.5f ? 1.5f : position.y;
        position.y = position.y > 5.4f ? 5.4f : position.y;
        position.x = position.x < -19.2f ? -19.2f : position.x;
        position.x = position.x > -5.3f ? -5.3f : position.x;
        position.z = position.z < -59.3f ? -59.3f : position.z;
        rotation.y = -scene.cursor.yaw - 90.f;
        rotation.x = scene.cursor.pitch;
    }

    generateModelMatrix();

    return true;
}


void Popup::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera.projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera.viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture[id]);
    shader->setUniform("LightColor",scene.lightColor);

    mesh->render();
}
