#include "fcamera.hpp"
#include "fscene.hpp"
#include <glm/gtx/transform.hpp>

FCamera::FCamera(float aspect, float fov, FNode* parent) : FKinetic(parent) {
    _fov = fov;
    _near = 0.1f;
    _far = 100.0f;
    _aspect = aspect;
    _shakeSpeed = 0.01f;
    _zoomSpeed = 1.0f;
    _move_speed = 3;
    _view_speed = 3;
}

void FCamera::setAspect(float aspect) {
    this->_aspect = aspect;
}

void FCamera::setNearFar(float near, float far) {
    _near = near;
    _far = far;
}

glm::mat4 FCamera::view() {
    return glm::lookAt(position(), position() + front(), upward());
}

glm::mat4 FCamera::projection() {
    return glm::perspective(glm::radians(_fov), _aspect, _near, _far);
}

void FCamera::mouseScroll(float y) {
    _fov = glm::clamp(_fov - y * _zoomSpeed, 10.0f, 80.0f);
}

void FCamera::keyPress(int key) {
    FKinetic::keyPress(key);
    if (key == GLFW_KEY_Q) {
        enabled = !enabled;
    }
}

void FCamera::render(FScene *scene) {
    scene->program().set("camera_pos", position());
    scene->program().set("camera_trans", projection() * view());
    scene->program().set("camera_view", view());
    scene->program().set("camera_projection", projection());
}
