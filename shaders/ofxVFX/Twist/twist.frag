#version 400
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform float uRand;
uniform float uVal1;
uniform float uVal2;
uniform float uVal3;

void main()
{
    vec2 uv = vPosition.xy;
    
    float x = max(uResolution.x*3, min(uv.x, uv.x + sin(uv.y / (153.25 * uRand * uRand) * uRand + uRand * uVal2 + uTime * 2.0) * uVal3));
    float y = max(uResolution.y*3, min(uv.y, uv.y + cos(uv.x / (251.57 * uRand * uRand) * uRand + uRand * uVal2 + uTime * 1.2) * uVal3 - uResolution.y*3));
    vec2 UV = vec2(x, y);
    
    vec4 col = texture(uBase, UV);
    fragColor = col;
}
