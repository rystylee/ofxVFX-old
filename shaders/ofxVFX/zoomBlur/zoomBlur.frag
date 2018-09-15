#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uVal1;

// -----------------------------------------
// https://wgld.org/d/webgl/w067.html
// -----------------------------------------

const float n = 30.0;

float random(vec3 scale, float seed)
{
    return fract(sin(dot(gl_FragCoord.stp + seed, scale)) * 43758.5453 + seed);
}

void main()
{
    vec2 uv = vPosition.xy;
    
    vec2 center = vec2(uResolution * 0.5);
    vec3 destColor = vec3(0.0);
    float rand = random(vec3(12.9898, 78.233, 151.7182), 0.0);
    vec2 centerOffset = uv - center;
    float totalWeight = 0.0;

    float strength = uVal1; // 0.0 ~ 1.0
    float nFrag = 1.0 / n;
    for(float i=0.0; i<=n; i++)
    {
        float percent = (i + rand) * nFrag;
        float weight = percent - percent * percent;
        vec2  t = uv - centerOffset * percent * strength * 0.3;
        destColor += texture(uBase, t).rgb * weight;
        totalWeight += weight;
    }
    
    fragColor = vec4(destColor / totalWeight, 1.0);
}
