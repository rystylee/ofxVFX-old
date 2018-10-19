#version 410
precision mediump float;

in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D uBlur;
uniform vec2 uResolution;
uniform float uGamma;

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;

    vec3 hdrColor = texture(tex, uv).rgb;
    vec3 bloomColor = texture(uBlur, uv).rgb;
    hdrColor += bloomColor;

    vec3 result = pow(hdrColor, vec3(1.0 / uGamma));
    fragColor = vec4(result, 1.0);
}
