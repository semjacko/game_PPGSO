#ifndef animal_H
#define animal_H


#include "object.h"
#include "scene.h"
#include "animator.h"


class Animal final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


    Animator animator;

public:
    bool animate = true;


    Animal(glm::vec3 p, glm::vec3 r, glm::vec3 s, Animator animator);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif