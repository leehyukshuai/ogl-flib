#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 camera_view;
uniform mat4 camera_projection;
out vec3 textureDir;

void main() {
    textureDir = aPos;
    vec4 pos = (camera_projection * mat4(mat3(camera_view)) * vec4(aPos, 1.0));
    gl_Position = pos.xyww;
}
