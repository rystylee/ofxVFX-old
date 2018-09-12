#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uVal1;

float rnd(vec3 scale, float seed)
{
    return fract(sin(dot(gl_FragCoord.stp + seed, scale)) * 43758.5453 + seed);
}

void main()
{
    vec2 uv = vPosition.xy;
    
    float tFrag = 1.0;
    float nFrag = 1.0 / 30.0;
    vec2 centerOffset = vec2(uResolution * 0.5);
    
    vec3  destColor = vec3(0.0);
    float random = rnd(vec3(12.9898, 78.233, 151.7182), 0.0);
    vec2  fc = uv;
    vec2  fcc = fc - centerOffset;
    float totalWeight = 0.0;
    
    for(float i=0.0; i<=30.0; i++)
    {
        float percent = (i + random) * nFrag;
        float weight = percent - percent * percent;
        vec2  t = fc - fcc * percent * uVal1 * 100.0 * 0.1 * nFrag;
        destColor += texture(uBase, t * tFrag).rgb * weight;
        totalWeight += weight;
    }
    
    fragColor = vec4(destColor / totalWeight, 1.0);
}
