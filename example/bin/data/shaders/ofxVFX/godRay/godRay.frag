#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
const float uExposure = 1.0;
const float uDecay = 0.8;
const float uDensity = 1.0;
const float uWeight = 1.0;
const vec2 uLightPositionOnScreen = vec2(960, 540);

const int NUM_SAMPLES = 100;

// -----------------------------------------
// http://fabiensanglard.net/lightScattering/
// -----------------------------------------

void main()
{
    vec2 uv = vPosition.xy;
    
    vec2 deltaUV = vec2(uv - uLightPositionOnScreen);
    deltaUV *= 1.0 / float(NUM_SAMPLES) * uDensity;
    float illuminationDecay = 1.0;
    
    vec3 c = vec3(0.0);
    for(int i=0; i<NUM_SAMPLES; i++)
    {
        uv -= deltaUV;
        vec3 smp = texture(uBase, uv).rgb;

        smp *= illuminationDecay * uWeight;
        
        c += smp;
        
        illuminationDecay *= uDecay;
    }
    
    fragColor = vec4(vec3(c * uExposure), 1.0);
}
