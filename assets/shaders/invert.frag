#version 330 core

in vec2 texCoord;
uniform sampler2D container;

void main() {
    gl_FragColor = 1 - texture2D(container, texCoord);
}
