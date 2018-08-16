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

const float timeScale = 7.5;

void main()
{
    vec2 uv = vPosition.xy;
    
    vec4 col = texture(uBase, uv);
    
    //vec3 noise = vec3(shadeVal1, 0.0, u_time * 1.5);
    //vec3 noise = vec3(0.025, 0.0, shadeVal1*10.0 + u_time);
    vec3 noise = vec3(0.1, 0.01, uTime * timeScale);
    
    float r = snoise(vec3(noise.x * uv.x, noise.y * uv.y, noise.z));

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
    
    fragColor = vec4(col);
}
