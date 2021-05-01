#include "object.h"


void Object::generateModelMatrix() {
    glm::mat4 t = glm::translate(position);
    glm::mat4 r = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    glm::mat4 s = glm::scale(scale);

    modelMatrix = t * r * s;
}


// this needs to be called everytime when the position changes
void Object::updateCollider() {
    collider[0] = collisionPoints[0];
    collider[1] = collisionPoints[1];

    collider[0] *= scale;
    collider[1] *= scale;

    // collider need to transforms with its object
    glm::mat4 t = glm::translate(collider[0]);
    glm::mat4 r = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    glm::mat4 m = r * t;
    collider[0] = {m[3][0], m[3][1], m[3][2]};
    t = glm::translate(collider[1]);
    r = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    m = r * t;
    collider[1] = {m[3][0], m[3][1], m[3][2]};

    collider[0] += position;
    collider[1] += position;

    // make sure collider[0] hold smaller coordinates and collider[1] higher coordinates for each of {x, y, z}
    for (int i = 0; i < 3; i++) {
        if (collider[0][i] > collider[1][i]) {
            std::swap(collider[0][i], collider[1][i]);
        }
    }
}


bool Object::isColliding(glm::vec3 *objCollider) {
    return (objCollider[0].x <= collider[1].x && objCollider[1].x >= collider[0].x) &&
           (objCollider[0].y <= collider[1].y && objCollider[1].y >= collider[0].y) &&
           (objCollider[0].z <= collider[1].z && objCollider[1].z >= collider[0].z);
}


// this finds out, on which side the collision occurred and returns normal of that side
glm::vec3 Object::getCollisionNormal(glm::vec3 *objCollider) {
    float diffs[6] = {
            std::abs(objCollider[0].x - collider[1].x),
            std::abs(objCollider[0].y - collider[1].y),
            std::abs(objCollider[0].z - collider[1].z),
            std::abs(objCollider[1].x - collider[0].x),
            std::abs(objCollider[1].y - collider[0].y),
            std::abs(objCollider[1].z - collider[0].z)
    };

    glm::vec3 normals[6] = {
            {1,0,0},
            {0,1,0},
            {0,0,1},
            {-1,0,0},
            {0,-1,0},
            {0,0,-1}
    };

    int minIndex = 0;
    for (int i = 1; i < 6; i++) {
        if (diffs[i] < diffs[minIndex]) {
            minIndex = i;
        }
    }

    return normals[minIndex];
}