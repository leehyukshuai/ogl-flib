#version 330 core

// position and direction are in WORLD coordinates!
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 camera_pos;
uniform samplerCube skybox;

void main() {
    vec3 I = fragPos - camera_pos;
    vec3 R = reflect(I, normal);
    gl_FragColor = textureCube(skybox, R);
}
