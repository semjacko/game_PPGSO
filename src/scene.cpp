#include "scene.h"
#include "popup.h"
#include "animal.h"
#include "cloud.h"


Scene::Scene(Camera camera) {
    this->camera = camera;
    scene1();
}


uint8_t Scene::getId() {
    return sceneId;
}


void Scene::scene1() {  // initialize first scene
    player.position = {0,0.01,-1};
    player.rotation = {0,180,0};
    lightPosition = {0, 5.5, 0};
    LColors[0] = {1, 1, 1};
    currColor = 0;

    glm::vec3 p = {0, -0.5, 0}; // floor
    glm::vec3 r = {0, 0, 0};
    glm::vec3 s = {9, 1, 15};
    auto obj = std::make_unique<SimpleObject>(p, r, s, 2);
    objects.push_back(move(obj));

    p = {0, 6, 0}; // ceiling
    r = {0, 0, 0};
    s = {9, 1, 15};
    obj = std::make_unique<SimpleObject>(p, r, s, 3);
    objects.push_back(move(obj));

    p = {5, 2.75, 0}; // right wall
    r = {0, 0, 0};
    s = {1, 7.5, 17};
    obj = std::make_unique<SimpleObject>(p, r, s, 0);
    objects.push_back(move(obj));

    p = {-5, 2.75, 0}; // left wall
    r = {0, 0, 0};
    s = {1, 7.5, 17};
    obj = std::make_unique<SimpleObject>(p, r, s, 0);
    objects.push_back(move(obj));

    p = {0, 2.75, 8}; // front wall
    r = {0, 0, 0};
    s = {9, 7.5, 1};
    obj = std::make_unique<SimpleObject>(p, r, s, 0);
    objects.push_back(move(obj));

    p = {0, 2.75, -8}; // back wall
    r = {0, 0, 0};
    s = {9, 7.5, 1};
    obj = std::make_unique<SimpleObject>(p, r, s, 0);
    objects.push_back(move(obj));

    p = {0, 0, 7.5}; // doors
    r = {-90, 0, 180};
    s = {0.02, 0.02, 0.02};
    obj = std::make_unique<SimpleObject>(p, r, s, 9);
    objects.push_back(move(obj));

    p = {1.5, 1, -5}; // bed
    r = {-90, -90, 0};
    s = {0.025, 0.025, 0.025};
    obj = std::make_unique<SimpleObject>(p, r, s, 8);
    objects.push_back(move(obj));

    p = {0, 5.5, 0}; // lamp
    r = {0, 0, 0};
    s = {0.04, 0.04, 0.04};
    auto obj4 = std::make_unique<SimpleObject>(p, r, s, 7);
    objects.push_back(move(obj4));
}


void Scene::scene2() {  // initialize second scene
    player.position = {0,0.01,-3};
    player.rotation = {0,180,0};
    cursor.yaw = -90.f;
    cursor.pitch = 0.f;
    camera.back = {0, 0, 1};
    lightPosition = {80, 200, -60};
    LColors[0] = {1, 1, 0.8};
    currColor = 0;

    glm::vec3 p;
    glm::vec3 r;
    glm::vec3 s;

    for (int i = -20; i <= 16; i += 2) {  // grass
        for (int j = -60; j <= -4; j += 2) {
            float x = glm::linearRand(0.f, 2.f);
            float z = glm::linearRand(0.f, 2.f);
            if (i + x < -5 && j + z < -33) { // skip the cave
                continue;
            }
            p = {i + x, 0, j + z};
            r = {0, 0, 0};
            float scale = glm::linearRand(1.f, 3.f);
            s = {scale,scale,scale};
            auto grass = std::make_unique<SimpleObject>(p, r, s, 5);
            objects.push_back(move(grass));
        }
    }

    for (int i = 0; i < 4; i++) {  // cliffs
        // right and left side
        float z = -5 - 18 * i;
        p = {20, -1, z};
        r = {0, 180, 0};
        s = {1,1,1};
        auto cliff = std::make_unique<SimpleObject>(p, r, s, 6);
        objects.push_back(move(cliff));
        p = {-20, -1, z};
        r = {0, 0, 0};
        s = {1,1,1};
        cliff = std::make_unique<SimpleObject>(p, r, s, 6);
        objects.push_back(move(cliff));
    }
    for (int i = 0; i < 2; i++) {  // cliffs
        // front and back side
        float x = -11 + 21 * i;
        p = {x, -1, 0};
        r = {0, 90, 0};
        s = {1,1,1.2};
        auto cliff = std::make_unique<SimpleObject>(p, r, s, 6);
        objects.push_back(move(cliff));
        p = {x, -1, -60};
        r = {0, -90, 0};
        s = {1,1,1.2};
        cliff = std::make_unique<SimpleObject>(p, r, s, 6);
        objects.push_back(move(cliff));
    }
    p = {10, -1, -20};  // cliff
    r = {0, 180, 0};
    s = {2,1,0.5};
    auto cliff = std::make_unique<SimpleObject>(p, r, s, 6);
    objects.push_back(move(cliff));

    p = {-12, -1, -34}; // cave
    r = {0, -90, 0};
    s = {0.5,1,0.8};
    cliff = std::make_unique<SimpleObject>(p, r, s, 6);
    objects.push_back(move(cliff));
    p = {-5, -1, -46};
    r = {0, 0, 0};
    s = {0.3,1,1};
    cliff = std::make_unique<SimpleObject>(p, r, s, 6);
    objects.push_back(move(cliff));
    p = {-5, 6, -50};
    r = {0, 0, 80};
    s = {0.3,2,1.2};
    cliff = std::make_unique<SimpleObject>(p, r, s, 6);
    objects.push_back(move(cliff));

    for (int i = 0; i < 40; i++) { // trees
        p = {0,0, 0};
        r = {0, 0, 0};
        s = {1,1,1};
        auto tree = std::make_unique<SimpleObject>(p, r, s, 4);
        bool intersect = true;
        while (intersect) {
            intersect = false;
            float x = glm::linearRand(-20.f, 20.f);;
            float z = glm::linearRand(0.f, -60.f);
            while (x < -5 && z < -33) { // skip the cave
                x = glm::linearRand(-20.f, 20.f);
                z = glm::linearRand(0.f, -60.f);
            }
            tree->position = {x,2, z};
            tree->updateCollider();
            for (auto &obj : objects) { // check for some collision with another objects
                if (obj.get()->isColliding(tree->collider)) {
                    intersect = true;
                    break;
                }
            }
        }
        objects.push_back(move(tree));
    }

    p = {0, -0.5, -30}; // ground
    r = {0, 0, 0};
    s = {40, 1, 60};
    auto ground = std::make_unique<SimpleObject>(p, r, s, 1);
    objects.push_back(move(ground));

    for (int i = -200; i <= 200; i += 40) {  // clouds
        for (int j = -200; j <= 200; j += 40) {
            float x = glm::linearRand(0.f, 30.f);
            float y = glm::linearRand(0.f, 10.f);
            float z = glm::linearRand(0.f, 30.f);
            p = {i + x, 30 + y, j + z};
            r = {0, 0, 0};
            float scale = glm::linearRand(1.f, 3.f);
            s = {scale,scale,scale};
            auto cloud = std::make_unique<Cloud>(p, r, s);
            objects.push_back(move(cloud));
        }
    }

    p = lightPosition; // sun
    r = {0, 0, 0};
    s = {10, 10, 10};
    auto sun = std::make_unique<SimpleObject>(p, r, s, 10);
    objects.push_back(move(sun));

    p = {0, 0, 0}; // penguin
    r = {0, 0, 0};
    s = {1,1,1};
    Keyframe keyframes[60]; // keyframes
    keyframes[0].time = 0.f;
    keyframes[0].position = {-21, 7, -21};
    keyframes[0].rotation = {0, 90, 0};
    keyframes[0].scale = {2, 2, 2};
    keyframes[1].time = 2.f;
    keyframes[1].position = {-21, 7, -21};
    keyframes[1].rotation = {0, 90, 0};
    keyframes[1].scale = {2, 2, 2};
    keyframes[2].time = 2.25f;
    keyframes[2].position = {-21, 8, -21};
    keyframes[2].rotation = {0, 0, 0};
    keyframes[2].scale = {2, 2, 2};
    keyframes[3].time = 2.5f;
    keyframes[3].position = {-21, 7, -21};
    keyframes[3].rotation = {0, -90, 0};
    keyframes[3].scale = {2, 2, 2};
    keyframes[4].time = 5.f;
    keyframes[4].position = {-21, 7, -21};
    keyframes[4].rotation = {0, -90, 0};
    keyframes[4].scale = {2, 2, 2};
    keyframes[5].time = 5.7f;
    keyframes[5].position = {-17, 11, -21};
    keyframes[5].rotation = {-120, -90, 0};
    keyframes[5].scale = {2, 2, 2};
    keyframes[6].time = 7.f;
    keyframes[6].position = {-13, 0, -21};
    keyframes[6].rotation = {-360, -90, 0};
    keyframes[6].scale = {2, 2, 2};
    keyframes[7].time = 8.f;
    keyframes[7].position = {-13, 0, -21};
    keyframes[7].rotation = {-360, -90, 0};
    keyframes[7].scale = {2, 2, 2};
    keyframes[8].time = 8.25f;
    keyframes[8].position = {-13, 1, -21};
    keyframes[8].rotation = {-360, -180, 0};
    keyframes[8].scale = {2, 2, 2};
    keyframes[9].time = 8.5f;
    keyframes[9].position = {-13, 0.01, -21};
    keyframes[9].rotation = {-360, -270, 0};
    keyframes[9].scale = {2, 2, 2};
    for (int i = 0; i < 20; i++) {
        float rotationZ = i%2 ? 15.f : -15.f;
        keyframes[10+i].time = 8.5 + (i+1)/2.f;
        keyframes[10+i].position = {-13 + i/2.f, 0.01, -21};
        keyframes[10+i].rotation = {-360, -270, rotationZ};
        keyframes[10+i].scale = {2, 2, 2};
    }
    int j = 29;
    // movement like in movie TENET (backward)
    for (int i = 0; i < 30; i++) {  // make new keyframes to create a palindrome from keyframes
        keyframes[30+i].time = keyframes[29+i].time + keyframes[j].time - keyframes[29-i].time;
        j -= int(i > 0);
        keyframes[30+i].position = keyframes[29-i].position;
        keyframes[30+i].rotation = keyframes[29-i].rotation;
        keyframes[30+i].scale = keyframes[29-i].scale;
    }
    auto animator = Animator(keyframes, 60);
    auto animal = std::make_unique<Animal>(p, r, s, animator);
    objects.push_back(move(animal));
}


void Scene::changeScene(uint8_t id) {
    auto i = std::begin(objects);

    while (i != std::end(objects)) {    // erase every object of the scene
        i = objects.erase(i);
    }

    if (id == 1) {
        scene1();
    }
    else if (id == 2) {
        scene2();
    }
}


bool Scene::update(float dTime) {
    if (transition > 0) {   // if there is transition of an awakening scene
        if (lightColor.x < LColors[currColor].x) {
            lightColor += LColors[currColor] * dTime * 0.5f;
        }
        else {
            lightColor = LColors[currColor];
            transition = 0;
        }
    }
    else if (transition < 0) {  // if there is transition of a outgoing scene
        lightColor -= LColors[currColor] * dTime * 0.5f;
    }

    if (transition < 0 && lightColor.x <= 0.f) {
        transition = -transition;
        sceneId++;
        changeScene(sceneId);
    }
    else if (transition == 2) {  // transition "2" the game ends
        transition = 2;
        return false;
    }

    if (sceneId == 1 && player.position.z < -2) {
        if (!popupExists) {
            auto popup = std::make_unique<Popup>(0);
            objects.push_back(move(popup));
            popupExists = true;
        }
        if (keys[0]) {
            transition = -1;
            keys[0] = false; // to not to switch light color
        }
    }
    else if (sceneId == 2 && player.position.z < -37) { // popup for second scene and cave (light darken)
        if (player.position.x < -7) {
            if (!popupExists) {
                auto popup = std::make_unique<Popup>(1);
                objects.push_back(move(popup));
                popupExists = true;
            }
            if (keys[0]) {
                transition = -2;
            }
        }
        else {
            popupExists = false;
        }

        if (player.position.z < -54) { // cave: darken light color
            float LColormult = std::max(std::min((11.f + player.position.x) / 8.f, 1.f), 0.2f);
            lightColor = LColors[currColor] * LColormult;
        }
    }
    else {
        popupExists = false;
    }

    if (sceneId == 1 && keys[0]) {  // change light color
        currColor = ++currColor >= colorsSize ? 0 : currColor;
        lightColor = LColors[currColor];
    }

    // update every object
    camera.update(*this, dTime);
    player.update(*this, dTime);
    auto i = std::begin(objects);
    while (i != std::end(objects)) {
        auto obj = i->get();
        if (!obj->update(*this, dTime))
            i = objects.erase(i);
        else
            ++i;
    }

    // to take these inputs only once per click
    keys[0] = false;
    cursor.wheel = false;
    cursor.left = false;

    return true;
}


void Scene::render() {
    player.render(*this);

    for ( auto& obj : objects )
        obj->render(*this);
}