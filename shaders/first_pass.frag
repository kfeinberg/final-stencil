#version 330 core

out vec3 fragColor;

in vec4 color;

void main() {
    fragColor = vec3(color.xyz);
}
