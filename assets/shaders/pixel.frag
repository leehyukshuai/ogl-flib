#version 330 core

in vec2 texCoord;
uniform sampler2D screenTexture;

void main() {
    gl_FragColor = texture2D(screenTexture, texCoord);
}
