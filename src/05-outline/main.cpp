#include "flib.hpp"

class App: public FSceneApp {
    FProgram outline_program;
    FCamera* camera;
    FLight* light;
    FModel* floor, * obj_1, * obj_2;

public:
    App() : FSceneApp(1800, 1200, "05-stencil") {
        FShader vertex_shader(fetch("shaders/scene.vert"), GL_VERTEX_SHADER);
        FShader fragment_shader(fetch("shaders/simple.frag"), GL_FRAGMENT_SHADER);
        outline_program.load({ vertex_shader, fragment_shader });

        obj_1 = new FModel(fetch("models/cube/cube.obj"));
        obj_2 = new FModel(fetch("models/cube/cube.obj"));

        floor = new FModel(fetch("models/cube/cube.obj"));
        floor->trans().translate({ 0,-1,0 }).scale({ 10,1,10 });

        camera = new FCamera(aspect());
        camera->trans().translate({ 0, 0.2, 1 });

        light = new FDirectLight({ 0.4,0.4,0.4 }, { 0.7,0.6,0.6 }, { 0.6,0.6,0.6 }, { -1.4, -1, 1 });

        scene.add(obj_1);
        scene.add(obj_2);
        scene.add(floor);
        scene.add(camera);
        scene.add(light);
        scene.begin();
        scene.begin(outline_program);

        // 立方体的所有面都是以逆时针定义的,所以可以启用面剔除选项以节省性能
        glEnable(GL_CULL_FACE);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }
protected:
    void loop() override {
        glClearColor(0.2, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.update(deltaTime());

        // 绘制地板
        {
            floor->setFlags(FNodeDefault);
            obj_1->setFlags(FNodeUnvisible);
            obj_2->setFlags(FNodeUnvisible);
            // 保证绘制地板的时候不会改变模板缓存
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilMask(0x00);
            scene.render();
        }

        // 绘制箱体1
        {
            floor->setFlags(FNodeUnvisible);
            obj_1->setFlags(FNodeDefault);
            // 设置所有绘制的片段模板缓存为1
            obj_1->trans().stranslate({ 2,1.1,-3 });
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilMask(0xff);
            scene.render();
            // 绘制轮廓线
            obj_1->trans().scale({ 1.2,1.2,1.2 });
            FProgram old_program = scene.reset(outline_program);
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            scene.render();
            glEnable(GL_DEPTH_TEST);
            scene.reset(old_program);
            glStencilMask(0xff);
            glClear(GL_STENCIL_BUFFER_BIT);
        }

        // 绘制箱体2
        {
            obj_1->setFlags(FNodeUnvisible);
            obj_2->setFlags(FNodeDefault);
            // 设置所有绘制的片段模板缓存为1
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilMask(0xff);
            obj_2->trans().stranslate({ -2,1.1,-3 });
            scene.render();
            // 绘制轮廓线
            obj_2->trans().scale({ 1.2,1.2,1.2 });
            FProgram old_program = scene.reset(outline_program);
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            scene.render();
            glEnable(GL_DEPTH_TEST);
            scene.reset(old_program);
            glStencilMask(0xff);
            glClear(GL_STENCIL_BUFFER_BIT);
        }

    }
};

int main() {
    App().run();
}
