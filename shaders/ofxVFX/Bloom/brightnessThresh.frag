#version 410
precision mediump float;

in vec4 vColor;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 uResolution;
uniform float uBrightnessThresh;

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;
    
    vec4 color = texture(tex, uv);
    fragColor = max(color - vec4(vec3(uBrightnessThresh), 0.0), vec4(0.0));
}
