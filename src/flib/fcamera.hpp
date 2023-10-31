#pragma once
#include "fheaders.hpp"
#include "fkinetic.hpp"

class FCamera : public FKinetic {
private:
    float _shakeSpeed, _zoomSpeed;
    float _fov, _aspect, _near, _far;

protected:
    void mouseScroll(float y) override;
    void keyPress(int key) override;
    void render(FScene *scene) override;

public:
    FCamera(float aspect, float fov = 45.0f, FNode* parent = nullptr);

    void setAspect(float aspect);
    void setNearFar(float near, float far);
    glm::mat4 view();
    glm::mat4 projection();
};
