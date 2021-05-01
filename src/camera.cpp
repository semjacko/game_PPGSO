#include "camera.h"
#include "scene.h"


Camera::Camera(float fov, float ratio, float near, float far) {
    projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
}


void Camera::handleCursor(Cursor cursor) {
    if (cursor.wheel) { // change point of view between 1st and 3rd person
        animate = true;
        diff = firstPerson ? 1.5f : 4.f;
        firstPerson = !firstPerson;
    }

    glm::vec3 front;
    float yaw = cursor.yaw;
    float pitch = cursor.pitch;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->back = -glm::normalize(front);
}


// NOTE: in "player" class, the camera position is set to the player position
void Camera::update(Scene &scene, float dTime) {
    handleCursor(scene.cursor);

    if (animate) {  // if there is transition between POVs
        if (firstPerson) {
            diff -= dTime*2;
            if (diff <= 1.5f) {
                diff = 1.5f;
                animate = false;
            }
        }
        else {
            diff += dTime*2;
            if (diff >= 4.f) {
                diff = 4.f;
                animate = false;
            }
        }

        // this will make animated transition between POVs
        position += back * diff;    // the diff increases or decreases (depending on the POV) over time

        if (position.y < 0.f) {
            position.y = 0.f;
        }
        position.y += 1;
    }
    else {
        if (firstPerson) {
            position.y = 2.f;
        } else {
            position += back * 4.f; // here it is moved slightly backward from player position (3rd person)
            if (position.y < 0.f) {
                position.y = 0.f;
            }
            position.y += 1;
        }
    }

    viewMatrix = lookAt(position, position - back, up);
}

