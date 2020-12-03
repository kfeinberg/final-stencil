#version 330 core

in vec3 color;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex;
uniform bool useTexture;

void main() {
    if (useTexture) {
        vec4 texColor = texture(tex, uv);
        if (texColor.a < 0.1) {
            discard;
        }
        fragColor = texColor;
    } else {
        fragColor = vec4(color, 1.f);
    }
}
