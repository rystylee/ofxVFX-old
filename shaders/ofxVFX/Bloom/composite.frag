#version 400
precision mediump float;

in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2DRect tex;
uniform sampler2DRect uBlur;
uniform float uGamma;

void main()
{
    vec2 uv = vTexCoord.xy;

    vec3 hdrColor = texture(tex, uv).rgb;
    vec3 bloomColor = texture(uBlur, uv).rgb;
    hdrColor += bloomColor;

    vec3 result = pow(hdrColor, vec3(1.0 / uGamma));
    fragColor = vec4(result, 1.0);
}
