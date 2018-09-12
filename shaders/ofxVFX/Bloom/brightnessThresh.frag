#version 410
precision mediump float;

in vec4 vColor;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2DRect tex;
uniform float uBrightnessThresh;

void main()
{
    vec4 color = texture(tex, vTexCoord.xy);
    fragColor = max(color - vec4(vec3(uBrightnessThresh), 0.0), vec4(0.0));
}
