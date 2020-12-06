#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates

out vec2 texCoord0; // copying github, see if this works

// Transformation matrices
uniform mat4 model;             // model matrix
uniform mat4 view;              // view matrix
uniform mat4 projection;        // projection matrix

void main() {
    gl_Position = position; //set equal to position, do other
}
