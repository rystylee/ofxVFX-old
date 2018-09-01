#version 400 core
precision highp float;

uniform sampler2DRect uPrevBuffer;
uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform int uCount;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

// --------------------------------------------------------
// reference : https://www.shadertoy.com/view/4t3GDj
// --------------------------------------------------------

vec2 speed = vec2(1.0,2.0);
float v = 30.0;
float dist = 0.3;
float random1 = 1.0;
float random2 = 100.0;

float hash(float n)
{
    return fract(sin(dot(vec2(n,n) ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 turbulence(vec2 uv)
{
    vec2 turb;
    turb.x = sin(uv.x);
    turb.y = cos(uv.y);
    
    for(int i = 0; i < 10; i++)
    {
        float ifloat = 1.0 + float(i);
        float ifloat1 = ifloat + random1;
        float ifloat2 = ifloat + random2;
        
        float r1 = hash(ifloat1)*2.0-1.0;
        float r2 = hash(ifloat2)*2.0-1.0;
        
        vec2 turb2;
        turb2.x = sin(uv.x * (1.0 + r1*v) + turb.y * dist*ifloat + uTime * speed.x*r2);
        turb2.y = cos(uv.y * (1.0 + r1*v) + turb.x * dist*ifloat + uTime * speed.y*r2);
        
        turb.x = mix(turb.x, turb2.x, 0.5);
        turb.y = mix(turb.y, turb2.y, 0.5);
    }
    
    return turb;
}

float sampleDistance = 30.0;
float diffusion = 10.0;
float turbulenceStrength = 0.2;
float fluidify = 0.0001;
float attenuate = 0.5;

void main()
{
    vec2 uv = vPosition.xy;

    vec4 base = texture(uBase, uv);
    float keying = smoothstep(0.0, 1.0, distance(base.xyz, vec3(13.0 / 255.0, 163.0 / 255.0, 37.0 / 255.0)) * 1.0);
    vec4 baseColor = vec4(clamp(base.xyz + vec3(0.8), vec3(0.0), vec3(1.0)) * keying, keying);
    float grayColor = dot(base.rgb, vec3(0.298912, 0.586611, 0.114478));
    baseColor = vec4(vec3(grayColor), base.a);
    
    vec2 sDist = vec2(sampleDistance);
    
    vec2 turbuv = uv / uResolution.xy;
    turbuv.x *= (uResolution.x / uResolution.y);
    vec2 turb = turbulence(turbuv * 1.0) * 0.5 + 0.5;
    turb = turb.xy * 2.0 - 1.0;
    
    vec4 newColor  = texture(uPrevBuffer, uv);
    vec4 newColor1 = texture(uPrevBuffer, uv + vec2( 1.0,  0.0) * sDist);
    vec4 newColor2 = texture(uPrevBuffer, uv + vec2(-1.0,  0.0) * sDist);
    vec4 newColor3 = texture(uPrevBuffer, uv + vec2( 0.0,  1.0) * sDist);
    vec4 newColor4 = texture(uPrevBuffer, uv + vec2( 0.0, -1.0) * sDist);
    
    vec4 newColor5 = texture(uPrevBuffer, uv + vec2( 1.0,  1.0) * sDist);
    vec4 newColor6 = texture(uPrevBuffer, uv + vec2(-1.0,  1.0) * sDist);
    vec4 newColor7 = texture(uPrevBuffer, uv + vec2( 1.0, -1.0) * sDist);
    vec4 newColor8 = texture(uPrevBuffer, uv + vec2(-1.0, -1.0) * sDist);
    
    vec2 t = (newColor1.x + newColor1.y + newColor1.z) / 3.0 * vec2(1.0, 0.0);
    t += (newColor2.x + newColor2.y + newColor2.z) / 3.0 * vec2(-1.0, 0.0);
    t += (newColor3.x + newColor3.y + newColor3.z) / 3.0 * vec2( 0.0, 1.0);
    t += (newColor4.x + newColor4.y + newColor4.z) / 3.0 * vec2( 0.0, -1.0);
    
    t += (newColor5.x + newColor5.y + newColor5.z) / 3.0 * vec2( 1.0, 1.0);
    t += (newColor6.x + newColor6.y + newColor6.z) / 3.0 * vec2(-1.0, 1.0);
    t += (newColor7.x + newColor7.y + newColor7.z) / 3.0 * vec2( 1.0, -1.0);
    t += (newColor8.x + newColor8.y + newColor8.z) / 3.0 * vec2(-1.0, -1.0);
    
    t /= 8.0;
    vec2 dir = vec2(t + turb * turbulenceStrength) * uTime * diffusion;
    
    vec4 res = texture(uPrevBuffer, uv + dir);
    
    vec4 c = mix(res, baseColor, clamp(baseColor.a * fluidify + attenuate, 0.0, 1.0));
    fragColor = c;
}
