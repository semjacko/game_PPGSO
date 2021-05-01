#include "animator.h"


Animator::Animator() {}


Animator::Animator(Keyframe *keyframes, int length) {
    for (int i = 0; i < length; i++) {  // makes copy of array "keyframes"
        this->keyframes[i] = keyframes[i];
    }

    this->keyframesLength = length;
}


Keyframe Animator::getCurrentKeyframe(float dt) {
    time += dt;

    // if it's time to change keyframe
    if (time >= keyframes[currKeyframe + 1].time) {
        currKeyframe++;
    }

    // if it's out of keyframes, starts from beginning
    if (currKeyframe >= keyframesLength - 1) {
        time = 0;
        currKeyframe = 0;
    }


    Keyframe keyframe;
    // time difference between current and next keyframe
    float timeDiff = keyframes[currKeyframe + 1].time - keyframes[currKeyframe].time;
    // time difference between the start time of the current keyframe and the current time
    float timeSinceLast = time - keyframes[currKeyframe].time;

    // interpolation: current keyframe + (transformation difference between keyframes * (time since last keyframe / time difference between keyframes))
    keyframe.position = keyframes[currKeyframe].position + (keyframes[currKeyframe + 1].position - keyframes[currKeyframe].position) * (timeSinceLast / timeDiff);
    keyframe.rotation = keyframes[currKeyframe].rotation + (keyframes[currKeyframe + 1].rotation - keyframes[currKeyframe].rotation) * (timeSinceLast / timeDiff);
    keyframe.scale = keyframes[currKeyframe].scale + (keyframes[currKeyframe + 1].scale - keyframes[currKeyframe].scale) * (timeSinceLast / timeDiff);

    return keyframe;
}
