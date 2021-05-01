#ifndef popup_H
#define popup_H


#include "scene.h"
#include "player.h"
#include "object.h"


class Popup final : public Object {
private:
    const static uint8_t POPUP_COUNT = 2;
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture[POPUP_COUNT];
    static char *textureName[POPUP_COUNT];

    int id;


public:
    Popup(int id);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;
};


#endif
