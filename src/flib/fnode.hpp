#pragma once
#include "fheaders.hpp"
#include "ftransform.hpp"
#include "fprogram.hpp"

class FScene;

enum FNodeFlags {
    FNodeDefault = 0b0000,
    FNodePausedSelf = 0b0001,
    // 注意FNodeUnvisibleSelf对于light和camera是无效的
    FNodeUnvisibleSelf = 0b0010,
    FNodePausedChildren = 0b0100,
    FNodeUnvisibleChildren = 0b1000,
    FNodeUnvisible = 0b1010,
    FNodePaused = 0b0101,
};

class FNode {
    friend class FScene;
protected:
    FNodeFlags _flags;
    std::string _name;
    FTransform _trans;
    FNode* _parent;
    std::list<FNode*> _children;
public:
    FNode(FNode* parent = nullptr);
    virtual ~FNode();
    void addChild(FNode* child);
    const std::string& name() const;
    void name(std::string newName);
    void setFlags(FNodeFlags flags);
    bool hasFlags(FNodeFlags flags) const;
    FTransform& trans();
    glm::vec3 position() const;
    virtual void update(float delta);
    virtual void render(FScene *scene);
    virtual void keyPress(int key);
    virtual void keyRelease(int key);
    virtual void mouseMove(float x, float y);
    virtual void mouseScroll(float y);
};
