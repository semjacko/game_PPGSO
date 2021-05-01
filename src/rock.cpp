#include "rock.h"
#include "animal.h"


std::unique_ptr<ppgso::Mesh> Rock::mesh;
std::unique_ptr<ppgso::Texture> Rock::texture;
std::unique_ptr<ppgso::Shader> Rock::shader;


Rock::Rock(glm::vec3 p, glm::vec3 r, glm::vec3 s, glm::vec3 dir) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("rock.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("rock.obj");

    position = p;
    position.y += 2; // offset
    rotation = r;
    scale = s;
    velocity = dir * glm::vec3{30, 30, 30};

    collisionPoints[0] = {0.5, -2, -2.2};
    collisionPoints[1] = {1.8, -0.7, -0.7};
    updateCollider();
}


bool Rock::update(Scene &scene, float dTime) {
    age += dTime;
    if (age > 20) {
        return false;
    }

    glm::vec3 lastPosition = position;
    velocity += glm::vec3{0,-9.8,0} * 1.5f * dTime;
    position += velocity * dTime;
    updateCollider();

    for (auto &obj : scene.objects) {
        if (obj.get() == this) continue;

        if (obj.get()->isColliding(collider)) {
            auto rock2 = dynamic_cast<Rock*>(obj.get());
            auto animal = dynamic_cast<Animal*>(obj.get());
            if (rock2) {
                rock2->velocity = velocity / 2.f;
            }
            if (animal) {
                animal->animate = !animal->animate;
            }
            position = lastPosition;
            glm::vec3 surfaceNormal = obj.get()->getCollisionNormal(collider);
            velocity = glm::reflect(velocity, surfaceNormal) / 2.f;
            break;
        }
    }

    generateModelMatrix();

    return true;
}


void Rock::render(Scene &scene) {
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
