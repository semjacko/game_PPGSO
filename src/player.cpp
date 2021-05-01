#include "player.h"
#include "scene.h"
#include "rock.h"


std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

struct rotationKeyframe {
    glm::vec3 rotation;
    float time;
} rotZkeyframes[4] = {{{0,0, 0}, 0}, {{0,0, 15}, 0.2}, {{0,0,-15}, .6}, {{0,0, 0}, .8}};


Player::Player() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("penguin.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("penguin.obj");

    scale = {1.5, 1.5, 1.5};
    position.y = 0.01;

    collisionPoints[0] = {0., 0.6, 0.};
    collisionPoints[1] = {0., 0.6, 0.};
}


glm::vec3 Player::handleKeys(bool keys[5], glm::vec3 back, float dTime) {    // pohyb na zaklade natocenia kamery
    glm::vec3 direction = -back;
    direction.y = 0;
    direction = glm::normalize(direction);
    glm::vec3 side = {direction.z, 0, -direction.x};

    const float speed = 5.f * dTime;
    glm::vec3 dPos = {0, 0, 0};

    if (keys[1]) {
        dPos += speed * direction;
    }
    if (keys[2]) {
        dPos -= speed * direction;
    }
    if (keys[3]) {
        dPos += side * speed;
    }
    if (keys[4]) {
        dPos -= side * speed;
    }

    return dPos;
}


bool Player::update(Scene &scene, float dTime) {
    if (scene.cursor.left) {
        glm::vec3 s = {0.15f, 0.15f, 0.15f};
        auto rock = std::make_unique<Rock>(position, rotation, s, -scene.camera.back);
        scene.objects.push_back(move(rock));
    }

    glm::vec3 deltaPosition = handleKeys(scene.keys, scene.camera.back, dTime);

    if (deltaPosition.x != 0 || deltaPosition.z != 0) {
        rotation.y = -scene.cursor.yaw + 90;
    }

    animationTime += dTime;

    if (animationTime >= rotZkeyframes[currKeyframe + 1].time) {
        currKeyframe++;
    }

    if (currKeyframe >= 3 || deltaPosition.x == 0 && deltaPosition.z == 0) {
        animationTime = 0;
        currKeyframe = 0;
    }

    float timeDiff = rotZkeyframes[currKeyframe + 1].time - rotZkeyframes[currKeyframe].time;
    float timeSinceLast = animationTime - rotZkeyframes[currKeyframe].time;
    glm::vec3 keyframeR = rotZkeyframes[currKeyframe].rotation + (rotZkeyframes[currKeyframe + 1].rotation - rotZkeyframes[currKeyframe].rotation) * (timeSinceLast / timeDiff);
    rotation.z = keyframeR.z;

    glm::vec3 lastPosition = position;
    position += deltaPosition;
    updateCollider();

    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        if (obj.get()->isColliding(collider)) {
            position = lastPosition;
            updateCollider();
            break;
        }
    }

    scene.camera.position = position;

    generateModelMatrix();
    
    return true;
}

void Player::render(Scene &scene) {
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