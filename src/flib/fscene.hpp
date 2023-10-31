#pragma once
#include "fheaders.hpp"
#include "flight.hpp"
#include "fmodel.hpp"
#include "fcamera.hpp"
#include "fprogram.hpp"
#include "fnode.hpp"

class FScene {
    friend class FSceneApp;
    std::unordered_map<FNode *, FTransform> _trans;
    FProgram _program;
    FNode* _root;
    // 它会在begin中被设置为指向实际的camera结点
    FCamera* _camera;
public:
    FScene();
    ~FScene();
    // added children will be released automated
    void add(FNode* node, FNode* parent = nullptr);
    // update不涉及bind到program的操作
    void update(float delta);
    const FProgram& program();
    // 返回旧的program
    FProgram reset(const FProgram& program);
    FTransform trans(FNode *node) const;
    FCamera& camera() const;
    // 绘制图形
    void render();
    // 初始化scene和自带的program
    void begin();
    // 初始化scene和指定的program
    void begin(const FProgram& other);
    void keyPress(int key);
    void keyRelease(int key);
    void mouseMove(float x, float y);
    void mouseScroll(float y);
};
