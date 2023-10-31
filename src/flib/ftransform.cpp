#include "ftransform.hpp"
#include <glm/gtx/transform.hpp>

FTransform::FTransform() : _matrix(glm::identity<glm::mat4>()) {}

FTransform::FTransform(glm::mat4 matrix) : _matrix(matrix) {}

FTransform& FTransform::reset() {
    _matrix = glm::mat4(1.0);
    return *this;
}

FTransform& FTransform::rotate(float radian_angle, glm::vec3 axis) {
    _matrix = glm::rotate(_matrix, radian_angle, axis);
    return *this;
}

FTransform& FTransform::translate(glm::vec3 vec) {
    _matrix = glm::translate(_matrix, vec);
    return *this;
}

FTransform& FTransform::scale(glm::vec3 vec) {
    _matrix = glm::scale(_matrix, vec);
    return *this;
}

FTransform& FTransform::srotate(float radian_angle, glm::vec3 axis) {
    _matrix = glm::rotate(radian_angle, axis);
    return *this;
}

FTransform& FTransform::stranslate(glm::vec3 vec) {
    _matrix = glm::translate(vec);
    return *this;
}

FTransform& FTransform::sscale(glm::vec3 vec) {
    _matrix = glm::scale(vec);
    return *this;
}

FTransform::operator glm::mat4() const {
    std::optional<int> o;
    return _matrix;
}

FTransform operator*(const FTransform& a, const FTransform& b) {
    return a._matrix * b._matrix;
}
