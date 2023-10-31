#include "fscene.hpp"
#include "futils.hpp"
#include "fskybox.hpp"
#include <typeinfo>

FScene::FScene() : _root(new FNode()) {
    FShader vertex_shader(fetch("shaders/scene.vert"), GL_VERTEX_SHADER);
    FShader fragment_shader(fetch("shaders/scene.frag"), GL_FRAGMENT_SHADER);
    _program = FProgram({ vertex_shader, fragment_shader });
    glUseProgram(_program);
}

FScene::~FScene() {
    delete _root;
}

void FScene::add(FNode* node, FNode* parent) {
    if (parent == nullptr) {
        parent = _root;
    }
    parent->_children.push_back(node);
    node->_parent = parent;
}

void FScene::update(float delta) {
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        if (!cur->hasFlags(FNodePausedSelf)) {
            cur->update(delta);
        }
        if (!cur->hasFlags(FNodePausedChildren)) {
            for (FNode* child : cur->_children) {
                stack.push(child);
            }
        }
    }
}

const FProgram& FScene::program() {
    return _program;
}

FProgram FScene::reset(const FProgram& program) {
    FProgram old_program = _program;
    _program = program;
    return old_program;
}

FTransform FScene::trans(FNode* node) const {
    if (_trans.find(node) == _trans.end()) {
        throw std::runtime_error("Error when searching node's trans.");
    } else {
        return _trans.find(node)->second;
    }
}

FCamera& FScene::camera() const {
    return *_camera;
}

void FScene::render() {
    glUseProgram(_program);

    // 先将所有结点的坐标系确定好
    std::stack<std::pair<FNode*, FTransform>> tstack;
    tstack.push({ _root, FTransform() });
    while (!tstack.empty()) {
        std::pair<FNode*, FTransform> cur = tstack.top();
        tstack.pop();
        if (!cur.first->hasFlags(FNodeUnvisibleSelf)) {
            _trans[cur.first] = cur.second; // ##BUG## 必须使用[]才能实现覆盖，insert会忽略
        }
        if (!cur.first->hasFlags(FNodeUnvisibleChildren)) {
            for (FNode* child : cur.first->_children) {
                tstack.push({ child, cur.second * cur.first->_trans });
            }
        }
    }

    // 渲染不透明的模型，将透明模型放入blenderModels中
    std::stack<FNode*> stack;
    stack.push(_root);
    std::multimap<float, FNode*, std::greater<float>> blenderModels;
    glm::vec3 camera_pos = _camera->position();
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        if (!cur->hasFlags(FNodeUnvisibleSelf)) {
            FModel* p = dynamic_cast<FModel*>(cur);
            if (p && static_cast<FModel*>(cur)->getBlender()) {
                // 透明的模型等到最后一步再做
                blenderModels.insert({ glm::distance(camera_pos, cur->position()), cur });
            } else {
                // 正常节点直接渲染就行
                cur->render(this);
            }
        }

        if (!cur->hasFlags(FNodeUnvisibleChildren)) {
            for (FNode* child : cur->_children) {
                stack.push(child);
            }
        }
    }

    // 绘制剩余的透明模型
    for (auto& p : blenderModels) {
        p.second->render(this);
    }
}

void FScene::begin() {
    glUseProgram(_program);
    // 设置灯光的数量并检查摄影机的数量
    int directLightNum = 0, pointLightNum = 0, spotLightNum = 0, cameraNum = 0;
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        if (dynamic_cast<FDirectLight*>(cur)) {
            cur->name("directLights[" + std::to_string(directLightNum++) + "]");
        } else if (dynamic_cast<FPointLight*>(cur)) {
            cur->name("pointLights[" + std::to_string(pointLightNum++) + "]");
        } else if (dynamic_cast<FSpotLight*>(cur)) {
            cur->name("spotLights[" + std::to_string(spotLightNum++) + "]");
        } else if (dynamic_cast<FCamera*>(cur)) {
            cameraNum++;
            _camera = static_cast<FCamera*>(cur);
        } else if (dynamic_cast<FSkyBox*>(cur)) {
            _program.set("skybox_exists", true);
            // _program.set("skybox", 0);   // no need to explictly declare
        }
        for (FNode* child : cur->_children) {
            stack.push(child);
        }
    }
    if (cameraNum != 1) {
        throw std::runtime_error("Error: Camera number is " + std::to_string(cameraNum) + ", only 1 is allowed");
    }
    if (directLightNum > 32 || pointLightNum > 32 || spotLightNum > 32) {
        throw std::runtime_error("Error: light number exceeded, please modify scene.frag and FScene::begin()");
    }
    if (directLightNum + pointLightNum + spotLightNum == 0) {
        std::cerr << "Since no lights are bound to scene, the scene may be very dark\n";
    }
    _program.set("directLightNum", directLightNum);
    _program.set("pointLightNum", pointLightNum);
    _program.set("spotLightNum", spotLightNum);
}

void FScene::begin(const FProgram& other) {
    FProgram old_program = _program;
    _program = other;
    begin();
    _program = old_program;
}

void FScene::keyPress(int key) {
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        cur->keyPress(key);
        for (FNode* child : cur->_children) {
            stack.push(child);
        }
    }
}

void FScene::keyRelease(int key) {
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        cur->keyRelease(key);
        for (FNode* child : cur->_children) {
            stack.push(child);
        }
    }
}

void FScene::mouseMove(float x, float y) {
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        cur->mouseMove(x, y);
        for (FNode* child : cur->_children) {
            stack.push(child);
        }
    }
}

void FScene::mouseScroll(float y) {
    std::stack<FNode*> stack;
    stack.push(_root);
    while (!stack.empty()) {
        FNode* cur = stack.top();
        stack.pop();
        cur->mouseScroll(y);
        for (FNode* child : cur->_children) {
            stack.push(child);
        }
    }
}
