#ifndef scene_H
#define scene_H


#include <list>
#include <memory>

#include "camera.h"
#include "simpleObject.h"
#include "object.h"
#include "player.h"


class Scene {
private:
    uint8_t sceneId = 1;

    bool popupExists = false;

    int transition = 1;

    glm::vec3 LColors[4] = {glm::vec3{1,1,1}, glm::vec3{0.6,0.6,1}, glm::vec3{0.6,1,0.6}, glm::vec3{1,0.6,0.6}};

    uint8_t colorsSize = 4;

    uint8_t currColor = 0;


    void changeScene(uint8_t id);


public:
    Camera camera;

    Player player;

    glm::vec3 lightPosition;

    glm::vec3 lightColor = {0, 0, 0};

    std::list<std::unique_ptr<Object>> objects;

    bool keys[5] = {0};

    Cursor cursor;


    Scene(Camera camera);

    uint8_t getId();

    void scene1();

    void scene2();

    bool update(float dTime);

    void render();
};


#endif