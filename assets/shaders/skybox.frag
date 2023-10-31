#version 330 core

in vec3 textureDir; // 代表3D纹理坐标的方向向量
uniform samplerCube cubemap; // 立方体贴图的纹理采样器

void main() {
    gl_FragColor = texture(cubemap, textureDir);
}