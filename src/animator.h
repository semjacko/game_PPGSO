#ifndef animator_H
#define animator_H


#include <glm/glm.hpp>


typedef struct Keyframe {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float time;
};


class Animator {
private:
    Keyframe keyframes[100];
    int keyframesLength;
    float time = 0;

public:
    int currKeyframe = 0;


    Animator();

    Animator(Keyframe *keyframes, int length);

    Keyframe getCurrentKeyframe(float dt);
};


#endif