#include "flib.hpp"

class App: public FSceneApp {
    FModel* nanosuit, * floor, * core;
    FCamera* camera;
    FLight* directLight, * spotLight;
    std::vector<std::pair<FLight*, FModel*>> pointLights;

public:
    App() : FSceneApp(1800, 1200, "02-scene") {
        // 创建基本的光源
        scene.add(new FDirectLight({ 0.1,0.1,0.1 }, { 0.2,0.2,0.2 }, { 0.5,0.5,0.5 }, { -2,-1,1 }));
        scene.add(new FDirectLight({ 0.1,0.1,0.1 }, { 0.8,0.8,0.8 }, { 0.5,0.5,0.5 }, { 2,-1,1 }));
        scene.add(new FSpotLight({ 0,0,0 }, { 2,1.5,0.0 }, { 2,0.6,0.6 }, { 0,-1,-1 }));

        // 创建地板模型
        floor = new FModel(fetch("models/cube/cube.obj"));
        floor->trans().translate({ 0, -4, 0 }).scale({ 100, 1, 100 });
        floor->setMaterial(FMaterial({ 0.0,0.0,0.0 }, { 0.5,0.5,0.5 }, { 0.2,0.2,0.2 }, 32.0f));
        scene.add(floor);

        // 创建核心模型
        core = new FModel(fetch("models/polygon/polygon.obj"));
        core->setMaterial(FMaterial({ 0,0,0 }, { 0,0,0 }, { 1,1,1 }, 128.0f));
        scene.add(core);

        // 创建人物模型
        nanosuit = new FModel(fetch("models/nanosuit_reflection/nanosuit.obj"));
        nanosuit->trans().translate({ 0, -3, -5 }).scale({ 0.6, 0.6, 0.6 });
        scene.add(nanosuit);
        // 创建点光源以及对应的光源模型 
        // FMaterial的第一个参数true代表在渲染这种材质的物体时,不需要考虑光照的影响
        std::vector<FMaterial> lightMaterials {
            { { 0.0, 0.0, 0.0 }, { 0.5,0.0,0.0 }, { 2.0, 0.0, 0.0 } },
            { { 0.0, 0.0, 0.0 }, { 0.0,0.5,0.0 }, { 0.0, 2.0, 0.0 } },
            { { 0.0, 0.0, 0.0 }, { 0.0,0.0,0.5 }, { 0.0, 2.0, 2.0 } },
            { { 0.0, 0.0, 0.0 }, { 0.0,0.0,0.5 }, { 2.0, 0.0, 2.0 } },
            { { 0.0, 0.0, 0.0 }, { 0.0,0.0,0.5 }, { 2.0, 2.0, 0.0 } },
            { { 0.0, 0.0, 0.0 }, { 0.0,0.0,0.5 }, { 0.0, 0.0, 2.0 } },
        };
        for (const FMaterial& material : lightMaterials) {
            auto pointLight = std::make_pair(new FPointLight(material), new FModel(fetch("models/polygon/polygon.obj")));
            pointLight.second->trans().scale({ 0.4,0.4,0.4 });
            pointLight.second->setMaterial(FMaterial(material._specular));
            pointLights.push_back(pointLight);
            scene.add(pointLight.first);
            scene.add(pointLight.second, pointLight.first); // 将model挂载到light上
        }

        // 创建摄影机和与之绑定在一起的聚光灯
        camera = new FCamera(aspect());
        spotLight = new FSpotLight({ 0.0,0.0,0.0 }, { 0.8,0.8,0.8 }, { 1.8,1.8,1.8 }, { 0,0,-1 }, 15.0f, 25.0f);
        scene.add(spotLight, camera);
        scene.add(camera);

        // 立方体的所有面都是以逆时针定义的,所以可以启用面剔除选项以节省性能
        glEnable(GL_CULL_FACE);

        // 在绘制前,必须要用scene.begin()初始化以绑定camera和lights
        scene.begin();
    }
    ~App() {
        // 什么都不用干,在scene被释放时,所有的结点都会被自动释放
    }
protected:
    void loop() override {
        static glm::vec3 bgColor = { 0.203, 0.203, 0.203 };
        glClearColor(bgColor.r, bgColor.g, bgColor.b, 2.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 利用ImGui控制背景颜色
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Background color:", glm::value_ptr(bgColor));
        ImGui::End();

        // 设置光源位置
        for (int i = 0; i < pointLights.size(); ++i) {
            auto& p = pointLights[i].first;
            switch (i % 6) {
            case 0:
                p->trans().srotate(currentTime(), { 0,1,0 }).translate({ 3, 3 * glm::cos(currentTime()), 0 });
                break;
            case 1:
                p->trans().srotate(currentTime() * 0.6, { 0.3,1,0 }).translate({ 2, 3 * glm::cos(currentTime() + 3), 0 });
                break;
            case 2:
                p->trans().srotate((currentTime() + 8) * 0.9, { -0.3,1,0 }).translate({ 4 + glm::cos(currentTime()), 3 * glm::cos(currentTime() - 2), 0 });
                break;
            case 3:
                p->trans().srotate((currentTime() - 2) * 0.4, { -0.1,0.3,0 }).translate({ 2, 1, 3 * glm::cos(currentTime() * 0.4) });
                break;
            case 4:
                p->trans().srotate((currentTime() - 6) * 0.8, { 0.1,1,0.1 }).translate({ glm::cos(currentTime() * 0.6 + 2), 3 * glm::cos(currentTime()) + 4, 0 });
                break;
            case 5:
                p->trans().srotate((currentTime() - 6) * 0.6, { 0.1,1,0.1 }).translate({ glm::cos(currentTime() * 0.6 + 2), 3 * glm::cos(currentTime()) + 4, 0 });
                break;
            }
        }

        // 调用所有结点的update函数
        scene.update(deltaTime());
        // 渲染所有节点
        scene.render();
    }
};

int main() {
    try {
        App app;
        app.run();
    } catch (std::runtime_error& e) {
        std::cerr << e.what();
    }
}