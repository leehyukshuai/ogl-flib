#include "futils.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

int main() {
    float dy = 30, dx = 60, dz = 0;
    float ry = glm::radians(dy);
    float rx = glm::radians(dx);
    float rz = glm::radians(dz);
    glm::mat4 my = glm::eulerAngleY(ry);
    glm::mat4 mx = glm::eulerAngleX(rx);
    glm::mat4 mz = glm::eulerAngleZ(rz);

    glm::mat4 euler_trans = my * mx * mz;

    glm::mat4 trans = glm::eulerAngleYXZ(ry, rx, rz);

    print(euler_trans);
    print(trans);

    float yaw, pitch, roll;
    glm::extractEulerAngleYXZ(trans, yaw, pitch, roll);
    std::cout << glm::degrees(yaw) << "," << glm::degrees(pitch) << "," << glm::degrees(roll) << "\n";
}