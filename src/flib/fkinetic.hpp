#pragma once
#include "fnode.hpp"

class FKinetic : public FNode {
protected:
    enum DIRECTION { FRONT, BACK, LEFT, RIGHT };
    bool _moves[4];
    float _move_speed, _view_speed;
    bool enabled;

    void update(float delta) override;
    void keyPress(int key) override;
    void keyRelease(int key) override;
    void mouseMove(float x, float y) override;
public:
    FKinetic(FNode* parent = nullptr);
    glm::vec3 front() const;
    glm::vec3 right() const;
    glm::vec3 upward() const;
    float pitch() const;
    float yaw() const;

    // float &move_speed();
    // float &view_speed();
};
