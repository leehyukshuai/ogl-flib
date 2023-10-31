#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model_trans;
uniform mat4 camera_trans;
uniform mat3 normal_trans;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

void main() {
    gl_Position = camera_trans * model_trans * vec4(aPosition, 1.0);

    fragPos = vec3(model_trans * vec4(aPosition, 1.0));
    normal = normalize(normal_trans * aNormal);
    texCoord = aTexCoord;
}
