#version 410
precision highp float;

uniform sampler2DRect uResult;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
    vec2 uv = vPosition.xy;
    
    vec4 result = texture(uResult, uv);
    fragColor = result;
}
