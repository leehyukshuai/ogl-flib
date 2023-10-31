#version 330 core

in vec2 texCoord;
uniform sampler2D screenTexture;

void main() {
    gl_FragColor = texture2D(screenTexture, texCoord);
    float average = (gl_FragColor.r * 0.2126  + gl_FragColor.g * 0.7152  + gl_FragColor.b * 0.0722 ) / 3.0;
    gl_FragColor = vec4(average, average, average, 1.0);
}
