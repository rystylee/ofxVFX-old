#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform float uTime;
uniform vec4 uColor;

void main()
{
    vec2 uv = vPosition.xy;
    vec4 col = texture(uBase, uv);
    
    col.r = 1.0 - col.r;
    col.g = 1.0 - col.g;
    col.b = 1.0 - col.b;
    
    fragColor = vec4(col.rgb, 1.0);
}
