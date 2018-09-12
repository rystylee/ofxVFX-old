#version 410
precision mediump float;

#pragma include "../utils/noise3D.glsl"

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform int uIsAdd;
uniform int uIsTwist;
uniform float uScalex;
uniform float uScaley;
uniform float uSpeed;
uniform vec4 uColor;

void main()
{
    vec2 uv = vPosition.xy;
    
    vec4 col = texture(uBase, uv);
    
    vec3 noise = vec3(uScalex, uScaley, uTime * uSpeed);
    float n = snoise(vec3(noise.x * uv.x, noise.y * uv.y, noise.z)); // -1 ~ 1

    if(uIsTwist != 0)
    {
        col = texture(uBase, uv + n * 100);
    }
    else
    {
        if(uIsAdd != 0)
        {
            col.r += n * uColor.r;
            col.g += n * uColor.g;
            col.b += n * uColor.b;
        }
        else
        {
            col.r *= n * col.r;
            col.g *= n * col.g;
            col.b *= n * col.b;
        }
    }
    
    fragColor = vec4(col.rgb, 1.0);
}
