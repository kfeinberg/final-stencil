#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex

out vec4 color; // position at color

void main() {
    color = vec4(0, 0, 0, 1); // set color to just black in first pass
    gl_Position = vec4(position,1.0);
}
