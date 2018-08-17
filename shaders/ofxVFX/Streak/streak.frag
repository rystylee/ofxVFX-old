#version 400
precision mediump float;

#pragma include "shaders/ofxVFX/utils/noise3D.glsl"

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;

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
    float r = snoise(vec3(noise.x * uv.x, noise.y * uv.y, noise.z)); // -1 ~ 1

    if(isTwist == 0)
    {
        col = texture(uBase, uv + r * 100);
    }
    else
    {
        vec3 c = vec3(1.0, 1.0, 1.0);
        if(isAdd == 0)
        {
            col.r += r * col.r;
            col.g += r * col.g;
            col.b += r * col.b;
        }
        else
        {
            col.r *= r * col.r;
            col.g *= r * col.g;
            col.b *= r * col.b;
        }
    }
    
    fragColor = vec4(col);
}
