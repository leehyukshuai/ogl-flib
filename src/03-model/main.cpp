#include "flib.hpp"
#include <glm/gtx/euler_angles.hpp>

#define FILEPATH "models/mermaid/mermaid.obj"

class App: public FSceneApp {
    FCamera* camera;
    FModel* model;
    FDirectLight* light;
    FPointLight* flash;
public:
    App() : FSceneApp(1800, 1200, "03-model") {
        scene.add(camera = new FCamera(aspect()));
        camera->trans().translate({ 0, 30, 10 });
        scene.add(light = new FDirectLight({ 0.2,0.2,0.2 }, { 0.6,0.6,0.6 }, { 0.3,0.3,0.3 }, { -1,-1.2,-1 }));
        scene.add(model = new FModel(fetch(FILEPATH)));
        scene.add(flash = new FPointLight({ 0,0,0 }, { 0.3,0.3,0.3 }, { 0.3,0.3,0.3 }), camera);
        scene.begin();
    }
protected:
    void loop() override {
        static float pi = glm::pi<float>();
        static bool useWireGrid = false;
        static glm::vec3 bgColor = { 0.9, 0.92, 0.95 };

        static glm::vec3 ambient{ 0.2, 0.2, 0.2 };
        static glm::vec3 diffuse{ 0.6, 0.6, 0.6 };
        static glm::vec3 specular{ 0.3, 0.3, 0.3 };
        static glm::vec3 direction{ -1, -1, 1 };

        static float pitch = 0, yaw = 0, roll = 0;
        static glm::vec3 scale{ 0.05, 0.05, 0.05 };

        glClearColor(bgColor.r, bgColor.g, bgColor.b, 2.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // 利用ImGui控制背景颜色
        ImGui::Begin("Settings");
        ImGui::Checkbox("Wire", &useWireGrid);
        ImGui::Text("Light:");
        ImGui::ColorEdit3("Background", glm::value_ptr(bgColor));
        ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specular));

        ImGui::Text("Model:");
        ImGui::SliderFloat("Yaw", &yaw, 0, 2 * pi);
        ImGui::SliderFloat("Pitch", &pitch, 0, 2 * pi);
        ImGui::SliderFloat("Roll", &roll, 0, 2 * pi);
        ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.001, 0.1);
        ImGui::End();


        model->trans() = glm::scale(glm::eulerAngleYXZ(yaw, pitch, roll), scale);
        light->setAmbient(ambient);
        light->setDiffuse(diffuse);
        light->setSpecular(specular);
        light->setDirection(direction);


        // 调用所有结点的update函数
        scene.update(deltaTime());
        // 渲染所有节点
        if (useWireGrid) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        scene.render();
    }
};

int main() {
    App().run();
}
