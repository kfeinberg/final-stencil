#version 330 core

in vec2 texCoord0;

out vec4 fragColor;

uniform sampler2D a;
uniform sampler2D b;

void main(void) {
    fragColor = texture(b, texCoord0);
}
