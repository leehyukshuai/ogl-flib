#include "flib.hpp"


class App: public FSceneApp {
    FModel* mermaid;
public:
    App() : FSceneApp(1800, 1200, "09-cubemap") {
        scene.reset(FProgram(fetch("shaders/scene.vert"), fetch("shaders/reflect.frag")));
        scene.add(new FCamera(aspect()));
        scene.add(new FDirectLight({ 0.1,0.24,0.3 }, { 0.2,0.3,0.3 }, { 0,0,0 }, { 1,-1.06,1.02 }));
        scene.add(new FSkyBox(fetch("images/skybox")));
        scene.add(mermaid = new FModel(fetch("models/mermaid/mermaid.obj")));
        mermaid->trans().translate({ 0,-15,-12 }).scale({ 0.03,0.03,0.03 });
        scene.begin();
    };
protected:
    void loop() override {
        glClear(GL_DEPTH_BUFFER_BIT);
        scene.update(deltaTime());
        scene.render();
    };
};


int main() {
    App app;
    app.run();
}