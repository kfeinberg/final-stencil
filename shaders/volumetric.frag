#version 330 core

in vec2 texCoord0; // uv coordinates??
in vec3 vertex; // position of vertex in 3D space

out vec4 fragColor; // color outputted to image

uniform sampler2D scene; // scene with objects occluded --> pass through first shader?
uniform vec3 sunPos; // position of sun on screen
uniform sampler2D godRaysSampler; // what is this? is this scene without objects occluded?

// constants: can be set or changed to uniform and set in scene
const float exposure = 0.3f;
const float decay = 0.96815;
const float density  = 0.926;
const float weight  = 0.587;
const int NUM_SAMPLES = 80;

// source: https://lokeshsharma97.wordpress.com/2017/08/04/god-rays-opengl/
void main(void) {
    vec2 tc = texCoord0.xy;
    vec2 deltatexCoord = (tc - (sunPos.xy*0.5 + 0.5));
    deltatexCoord *= 1.0/ float(NUM_SAMPLES);

    float illuminationDecay = 1.0f;

    vec4 godRayColor = texture(godRaysSampler, tc.xy)*0.4; // what is this??

    for(int i = 0 ; i< NUM_SAMPLES ; i++) {
            tc-= deltatexCoord;
            vec4 samp = texture(godRaysSampler , tc )*0.4; // what is this?
            samp *= illuminationDecay*weight;
            godRayColor += samp;
            illuminationDecay *= decay;
    }
    vec4 realColor = texture(scene, texCoord0);
    fragColor = ((vec4((vec3(godRayColor.r, godRayColor.g, godRayColor.b) * exposure), 1)) + (realColor*(1.0)));
}
