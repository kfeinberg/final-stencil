#version 330 core

in vec2 texCoord0;

out vec4 fragColor; // color outputted as reult

uniform vec3 sunPos; // position of sun on screen
uniform sampler2D regularScene; // real scene with real colors
uniform sampler2D occludedScene; // results from first pass, scene with objects occluded except sun

// constants: can be set or changed to uniform and set in scene
const float exposure = 0.2f;
const float decay = 0.96815;
const float density  = 0.926;
const float weight  = 0.587;
const int NUM_SAMPLES = 120;

// source: https://lokeshsharma97.wordpress.com/2017/08/04/god-rays-opengl/
void main(void) {
    vec2 tc = texCoord0.xy;
    //vec2 deltatexCoord = (tc - (sunPos.xy)); // TODO: which version?
    vec2 deltatexCoord = (tc - (sunPos.xy*0.5+0.5));
    deltatexCoord *= 1.0/ float(NUM_SAMPLES) * density;

    float illuminationDecay = 1.0f;

    vec4 godRayColor = texture(occludedScene, tc.xy)*0.4;

    for(int i = 0 ; i< NUM_SAMPLES ; i++) {
            tc-= deltatexCoord;
            vec4 samp = texture(occludedScene , tc )*0.4;
            samp *= illuminationDecay*weight;
            godRayColor += samp;
            illuminationDecay *= decay;
    }
    vec4 realColor = texture(regularScene, texCoord0.xy);
    fragColor = ((vec4((vec3(godRayColor.r, godRayColor.g, godRayColor.b) * exposure), 1)) + (realColor*(1.1)));
}
