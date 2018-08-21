#version 400
precision mediump float;

#pragma include "shaders/ofxVFX/utils/noise3D.glsl"

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform vec4 uColor;

// 0 == verticel, 1 == horizon1tal
const int direction = 1;
const int isAdd = 0;
const int isTwist = 1;
const float xscale = 0.01;
const float yscale= 0;
const float speed = 7.5;

void main()
{
    vec2 uv = vPosition.xy;
    
    vec4 col = texture(uBase, uv);
    
    vec3 noise = vec3(xscale, yscale, uTime * speed);
    float n = snoise(vec3(noise.x * uv.x, noise.y * uv.y, noise.z)); // -1 ~ 1

    if(isTwist == 0)
    {
        col = texture(uBase, uv + n * 100);
    }
    else
    {
        if(isAdd == 0)
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
