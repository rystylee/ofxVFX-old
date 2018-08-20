#version 400
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform float uVal1;
uniform float uVal2;

// ------------------------------------------------------
// reference : https://www.shadertoy.com/view/4sX3RN
// ------------------------------------------------------

const int isRandom = 0; // 0 is false

float mod289(float x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 perm(vec4 x)
{
    return mod289(((x * 34.0) + 1.0) * x);
}

float noise3d(vec3 p)
{
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

void main()
{
    vec2 uv = vPosition.xy;
    vec2 UV = uv / uResolution.xy;
    
    float val1 = (uVal1 * 2) - 1.0;
    float val2 = (uVal2 * 2) - 1.0;
    
    float v1 = 0.0; float v2 = 0.0;
    if(isRandom == 0)
    {
        v1 = 500 * cos(uTime) * noise3d(vec3(UV * 10.0, 0.0));
        v2 = 250 * sin(uTime) * noise3d(vec3(UV * 10.0, 1.0));
    }
    else
    {
        v1 = 400 * val1 * noise3d(vec3(UV * 10.0, 0.0));
        v2 = 300 * val2 * noise3d(vec3(UV * 10.0, 1.0));
    }

    vec2 n = vec2(v1, v2);
    vec4 color  = texture(uBase, min(uv + n, uResolution));
    
    fragColor = color;
}
