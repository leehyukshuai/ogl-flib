#include "flib.hpp"

class App: public FSceneApp {
public:
    App() : FSceneApp(1800, 1200, "07-blend") {
        FShader vertex_shader(fetch("shaders/scene.vert"), GL_VERTEX_SHADER);
        FShader fragment_shader(fetch("shaders/discard.frag"), GL_FRAGMENT_SHADER);
        FProgram program({ vertex_shader, fragment_shader });
        scene.reset(program);

        FModel* floor = new FModel(fetch("models/cube/cube.obj"));
        floor->trans().translate({ 0,-2,0 }).scale({ 10,1,10 });
        FCamera* camera = new FCamera(aspect());
        camera->trans().translate({ 0, 0.2, 1 });
        FLight* light = new FDirectLight({ 0.4,0.4,0.4 }, { 0.7,0.6,0.6 }, { 0.6,0.6,0.6 }, { -1, -1, 1 });
        // setBlender(true)会导致model的渲染次序在所有不透明物体之后进行
        FModel* window_1 = new FModel(fetch("models/window/window.obj"));
        window_1->setBlender(true);
        window_1->trans().translate({ 1, 0, 0 }).rotate(glm::radians(90.0f), { 0,1,0 });
        FModel* window_2 = new FModel(fetch("models/window/window.obj"));
        window_2->setBlender(true);
        window_2->trans().translate({ -1, 0, 0 }).rotate(glm::radians(90.0f), { 0,1,0 });
        FModel* window_3 = new FModel(fetch("models/window/window.obj"));
        window_3->setBlender(true);
        window_3->trans().translate({ 0, 0, 1 });
        FModel* window_4 = new FModel(fetch("models/window/window.obj"));
        window_4->setBlender(true);
        window_4->trans().translate({ 0, 0, -1 });

        FModel* grass_1 = new FModel(fetch("models/grass/grass.obj"));
        grass_1->trans().rotate(glm::radians(90.0f), { 0,1,0 });
        FModel* grass_2 = new FModel(fetch("models/grass/grass.obj"));
        grass_2->trans();

        // 你不需要考虑add的次序,因为物体的渲染次序将会在scene.render()中被妥善处理
        scene.add(window_1);
        scene.add(window_2);
        scene.add(window_3);
        scene.add(window_4);
        scene.add(grass_1);
        scene.add(grass_2);
        scene.add(floor);
        scene.add(camera);
        scene.add(light);
        scene.begin();
    }
protected:
    void loop() override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.3, 0.4, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.update(deltaTime());
        scene.render();
    }
};

int main() {
    App().run();
}
