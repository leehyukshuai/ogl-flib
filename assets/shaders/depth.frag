#version 330 core

uniform float near;
uniform float far;
uniform bool linear;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth;
    if(linear) {
        depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    } else {
        depth = gl_FragCoord.z;
    }
    gl_FragColor = vec4(vec3(depth), 1.0);
}