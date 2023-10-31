#include "fnode.hpp"

FNode::FNode(FNode* parent) : _flags(FNodeDefault) {
    if ((_parent = parent)) {
        parent->addChild(this);
    }
}

FNode::~FNode() {
    for (FNode* child : _children) {
        child->_parent = nullptr;
        delete child;
    }
    if (_parent) {
        _parent->_children.remove(this);
    }
}

void FNode::addChild(FNode* child) {
    _children.push_back(child);
}

const std::string& FNode::name() const {
    return _name;
}

void FNode::name(std::string newName) {
    _name = newName;
}

void FNode::setFlags(FNodeFlags flags) {
    _flags = flags;
}

bool FNode::hasFlags(FNodeFlags flags) const {
    return (_flags & flags) == flags;
}

FTransform& FNode::trans() {
    return _trans;
}

glm::vec3 FNode::position() const {
    return glm::vec3((glm::mat4)_trans * glm::vec4({ 0,0,0,1 }));
}

void FNode::update(float delta) {}

void FNode::render(FScene *scene) {}

void FNode::keyPress(int key) {}

void FNode::keyRelease(int key) {}

void FNode::mouseMove(float x, float y) {}

void FNode::mouseScroll(float y) {}
