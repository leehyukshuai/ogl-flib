#include "flib.hpp"


class App: public FSceneApp {
    FProgram default_program, refract_program, reflect_program;
    FModel* nanosuit_1, * nanosuit_2, * nanosuit_3;
    FCamera* camera;
public:
    App() : FSceneApp(1800, 1200, "09-cubemap") {
        default_program = scene.program();
        refract_program = FProgram(fetch("shaders/scene.vert"), fetch("shaders/refract.frag"));
        reflect_program = FProgram(fetch("shaders/scene.vert"), fetch("shaders/reflect.frag"));

        scene.add(new FDirectLight({ 1.0,1.0,1.0 }, { 1.0,1.0,1.0 }, { 1.0,1.0,1.0 }, { 1,-1.06,1.02 }));
        scene.add(camera = new FCamera(aspect(), 60));
        camera->trans().translate({ 0,0,12 });
        scene.add(new FSkyBox(fetch("images/skybox")));
        scene.add(nanosuit_1 = new FModel(fetch("models/nanosuit/nanosuit.obj")));
        nanosuit_1->trans().translate({ 0,-10,0 }).scale({ 1.3,1.3,1.3 });
        scene.add(nanosuit_2 = new FModel(fetch("models/nanosuit/nanosuit.obj")));
        nanosuit_2->trans().translate({ 13,-10,0 }).scale({ 1.3,1.3,1.3 });
        scene.add(nanosuit_3 = new FModel(fetch("models/nanosuit/nanosuit.obj")));
        nanosuit_3->trans().translate({ -13,-10,0 }).scale({ 1.3,1.3,1.3 });
        scene.begin();
        scene.begin(reflect_program);
        scene.begin(refract_program);

        // 立方体的所有面都是以逆时针定义的,所以可以启用面剔除选项以节省性能
        glEnable(GL_CULL_FACE);
    };
protected:
    void loop() override {
        glClear(GL_DEPTH_BUFFER_BIT);
        scene.update(deltaTime());

        nanosuit_1->setFlags(FNodeFlags::FNodeDefault);
        nanosuit_2->setFlags(FNodeFlags::FNodeUnvisible);
        nanosuit_3->setFlags(FNodeFlags::FNodeUnvisible);
        scene.reset(default_program);
        scene.render();

        nanosuit_1->setFlags(FNodeFlags::FNodeUnvisible);
        nanosuit_2->setFlags(FNodeFlags::FNodeDefault);
        nanosuit_3->setFlags(FNodeFlags::FNodeUnvisible);
        scene.reset(reflect_program);
        scene.render();

        nanosuit_1->setFlags(FNodeFlags::FNodeUnvisible);
        nanosuit_2->setFlags(FNodeFlags::FNodeUnvisible);
        nanosuit_3->setFlags(FNodeFlags::FNodeDefault);
        scene.reset(refract_program);
        scene.render();
    };
};


int main() {
    App app;
    app.run();
}