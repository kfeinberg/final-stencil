#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 5) in vec2 uv;   // Normal of the vertex

out vec2 texCoord0;

void main() {
    texCoord0 = uv;
    gl_Position = vec4(position, 1.f); //set equal to position, do other
}
