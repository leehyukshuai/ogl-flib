#include "flib.hpp"

class App: public FSceneApp {
    FProgram outline_program;
    FCamera* camera;
    FLight* light;
    FModel* floor, * grass_a, * grass_b;

public:
    App() : FSceneApp(1800, 1200, "06-discard") {
        FShader vertex_shader(fetch("shaders/scene.vert"), GL_VERTEX_SHADER);
        FShader fragment_shader(fetch("shaders/discard.frag"), GL_FRAGMENT_SHADER);
        FProgram program({ vertex_shader, fragment_shader });
        scene.reset(program);

        floor = new FModel(fetch("models/cube/cube.obj"));
        floor->trans().translate({ 0,-2,0 }).scale({ 10,1,10 });
        camera = new FCamera(aspect());
        camera->trans().translate({ 0, 0.2, 1 });
        light = new FDirectLight({ 0.4,0.4,0.4 }, { 0.7,0.6,0.6 }, { 0.6,0.6,0.6 }, { -1, -1, 1 });
        grass_a = new FModel(fetch("models/grass/grass.obj"));
        grass_a->trans().rotate(glm::radians(90.0f), { 0,1,0 });
        grass_b = new FModel(fetch("models/grass/grass.obj"));

        scene.add(floor);
        scene.add(grass_a);
        scene.add(grass_b);
        scene.add(camera);
        scene.add(light);
        scene.begin();
    }
protected:
    void loop() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.update(deltaTime());
        scene.render();
    }
};

int main() {
    App().run();
}
