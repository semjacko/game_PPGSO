#ifndef cloud_H
#define cloud_H


#include "object.h"
#include "scene.h"


class Cloud final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


public:
    Cloud(glm::vec3 p, glm::vec3 r, glm::vec3 s);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif
