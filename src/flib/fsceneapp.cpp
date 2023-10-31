#include "fsceneapp.hpp"

FSceneApp::FSceneApp(int width, int height, const std::string& title)
    : FApplication(width, height, title) {
    glEnable(GL_DEPTH_TEST);
}

void FSceneApp::loop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.update(deltaTime());
    scene.render();
}

void FSceneApp::key_callback(int key, int scankey, int action, int mods) {
    FApplication::key_callback(key, scankey, action, mods);
    if (action == GLFW_PRESS) {
        scene.keyPress(key);
    } else if (action == GLFW_RELEASE) {
        scene.keyRelease(key);
    }
}

void FSceneApp::mouse_move_callback(double x, double y) {
    scene.mouseMove(x, y);
}

void FSceneApp::scroll_callback(double x, double y) {
    scene.mouseScroll(y);
}

void FSceneApp::framebuffer_size_callback(int w, int h) {
    FApplication::framebuffer_size_callback(w, h);
    scene._camera->setAspect(w * 1.0 / h);
}
