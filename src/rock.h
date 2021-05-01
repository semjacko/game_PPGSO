#ifndef rock_H
#define rock_H


#include "scene.h"
#include "object.h"


class Rock final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0.f;  // celkovy cas existencie


public:
    glm::vec3 velocity = {0, 0, 0};


    Rock(glm::vec3 p, glm::vec3 r, glm::vec3 s, glm::vec3 dir);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif