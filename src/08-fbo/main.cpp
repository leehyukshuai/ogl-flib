#include "flib.hpp"

struct vertex {
    glm::vec2 pos;
    glm::vec2 tex;
};


class App: public FSceneApp {
    FId fbo, rbo, vao, vbo, ebo, texture;
    FProgram program;
    FModel* nanosuit, * floor, * core;
    FCamera* camera;
    FLight* directLight, * spotLight;
    std::vector<std::pair<FLight*, FModel*>> pointLights;

    float scale = 0.1;
    // 绘制的数据
    std::vector<vertex> vertices {
        { {-1., -1.}, { 0, 0 }},
        { {1., -1.}, {1, 0} },
        { {1., 1.}, {1, 1} },
        { {-1., 1.}, {0, 1} }
    };
    std::vector<unsigned> indices {
        0, 1, 2, 0, 2, 3
    };
public:
    App()
        : FSceneApp(1800, 1200, "08-fbo"),
        fbo(GL_FRAMEBUFFER), rbo(GL_RENDERBUFFER),
        vao(GL_VERTEX_ARRAY), vbo(GL_ARRAY_BUFFER),
        ebo(GL_ELEMENT_ARRAY_BUFFER), texture(GL_TEXTURE_2D) {

        // 创建framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // 创建并绑定到纹理附件
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width() * scale, height() * scale, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        // 创建并绑定到渲染缓冲对象
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width() * scale, height() * scale);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        // 检查framebuffer是否完全
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE");
        }
        // 恢复到默认的framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 创建一个vao并绑定上相应的vbo和ebo
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, tex));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        // 创建一个着色器
        program = FProgram(fetch("shaders/fbo.vert"), fetch("shaders/pixel.frag"));

        // 利用flib绘制场景
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
        nanosuit = new FModel(fetch("models/nanosuit/nanosuit.obj"));
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
protected:
    void loop() override {
        scene.update(deltaTime());
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


        // step 1
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.4, 0.5, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width() * scale, height() * scale);
        scene.render();
        glViewport(0, 0, width(), height());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // step 2
        glClearColor(0.4, 0.5, 0.7, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
    }
};

int main() {
    App app;
    app.run();
}