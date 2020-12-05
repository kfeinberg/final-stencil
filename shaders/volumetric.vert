#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex

out vec3 vertex; // point in scene
//out vec2 texCoord0; // copying github, see if this works

// Transformation matrices
uniform mat4 model;             // model matrix
uniform mat4 view;              // view matrix
uniform mat4 projection;        // projection matrix

void main() {
    vertex = ((view*model)*(vec4(position, 1.0))).xyz; // copied from metal.vert
    //texCoord0 = vec2((position.x+1.0)/2.0, (position.y+1.0)/2.0); // copying github, see if this works
    gl_Position = projection*view*model*(vec4(position,1.0));
}
