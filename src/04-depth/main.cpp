#include "flib.hpp"

class App: public FSceneApp {
public:
    App() : FSceneApp(1800, 1200, "04-depth") {
        FShader vertex_shader(fetch("shaders/depth.vert"), GL_VERTEX_SHADER);
        FShader fragment_shader(fetch("shaders/depth.frag"), GL_FRAGMENT_SHADER);
        FProgram program({ vertex_shader, fragment_shader });
        float near = 0.1f, far = 100.0f;

        program.set("near", near);
        program.set("far", far);
        program.set("linear", true);

        scene.reset(program);

        FCamera* camera = new FCamera(aspect());
        camera->setNearFar(near, far);
        scene.add(new FCamera(aspect()));

        scene.add(new FModel(fetch("models/legos/lego final.obj")));
        scene.begin();
    }
};

int main() {
    App().run();
}
