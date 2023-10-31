#pragma once
#include "fheaders.hpp"

class FTransform {
    friend class FModel;
    glm::mat4 _matrix;
public:
    FTransform();
    FTransform(glm::mat4 matrix);
    // 在之前transform的基础上进一步变形
    FTransform &reset();
    FTransform &rotate(float radian_angle, glm::vec3 axis);
    FTransform &translate(glm::vec3 vec);
    FTransform &scale(glm::vec3 vec);
    // 以s开头代表reset，重置transform
    FTransform &srotate(float radian_angle, glm::vec3 axis);
    FTransform &stranslate(glm::vec3 vec);
    FTransform &sscale(glm::vec3 vec);
    operator glm::mat4() const;
    friend FTransform operator *(const FTransform &a, const FTransform &b);
};