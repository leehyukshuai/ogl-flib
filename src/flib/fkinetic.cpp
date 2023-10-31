#include "fkinetic.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

FKinetic::FKinetic(FNode* parent) : FNode(parent), _move_speed(1.0f), _view_speed(1.0f), enabled(true) {
    for (int i = 0; i < 4; ++i) {
        _moves[i] = false;
    }
}

glm::vec3 FKinetic::front() const {
    return glm::normalize((glm::mat3)_trans * glm::vec3(0, 0, -1));
}

glm::vec3 FKinetic::right() const {
    return glm::normalize((glm::mat3)_trans * glm::vec3(1, 0, 0));
}

glm::vec3 FKinetic::upward() const {
    return glm::normalize((glm::mat3)_trans * glm::vec3(0, 1, 0));
}

float FKinetic::pitch() const {
    return glm::asin(front().y);
}

float FKinetic::yaw() const {
    float angle = glm::acos(glm::dot({ 0,1 }, glm::normalize(glm::vec2(front().x, -front().z))));
    return front().x > 0 ? -angle : angle;
}

void FKinetic::update(float delta) {
    if (enabled) {
        float dist = delta * _move_speed;
        if (_moves[FRONT]) {
            _trans = glm::translate(front() * dist) * _trans;
        }
        if (_moves[BACK]) {
            _trans = glm::translate(-front() * dist) * _trans;
        }
        if (_moves[RIGHT]) {
            _trans = glm::translate(right() * dist) * _trans;
        }
        if (_moves[LEFT]) {
            _trans = glm::translate(-right() * dist) * _trans;
        }
    }
}

void FKinetic::keyPress(int key) {
    if (enabled) {
        if (key == GLFW_KEY_W) {
            _moves[FRONT] = true;
        }
        if (key == GLFW_KEY_S) {
            _moves[BACK] = true;
        }
        if (key == GLFW_KEY_A) {
            _moves[LEFT] = true;
        }
        if (key == GLFW_KEY_D) {
            _moves[RIGHT] = true;
        }
    }
}

void FKinetic::keyRelease(int key) {
    if (key == GLFW_KEY_W) {
        _moves[FRONT] = false;
    }
    if (key == GLFW_KEY_S) {
        _moves[BACK] = false;
    }
    if (key == GLFW_KEY_A) {
        _moves[LEFT] = false;
    }
    if (key == GLFW_KEY_D) {
        _moves[RIGHT] = false;
    }
}

void FKinetic::mouseMove(float x, float y) {
    static bool initialized = false;
    static float lastX, lastY;
    if (!initialized) {
        lastX = x;
        lastY = y;
        initialized = true;
    }

    if (enabled) {
        float dx = x - lastX;
        float dy = y - lastY;
        lastX = x;
        lastY = y;

        // 计算旋转方向后新的pitch和yaw
        float _pitch = glm::clamp(pitch() - glm::radians(dy * _view_speed * 0.01f), glm::radians(-89.0f), glm::radians(89.0f));
        float _yaw = yaw() - glm::radians(dx * _view_speed * 0.01f);

        // 先移动到position，然后再旋转到pitch和yaw
        _trans = glm::translate(position()) * glm::eulerAngleYX(_yaw, _pitch);
    } else {
        initialized = false;
    }
}

// float& FKinetic::move_speed() {
//     return _move_speed;
// }
// float& FKinetic::view_speed() {
//     return _view_speed;
// }