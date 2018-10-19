#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2D uBase;
uniform vec2 uResolution;
uniform float uTime;

// ------------------------------------------------------
// reference : https://www.shadertoy.com/view/MdfXRs
// ------------------------------------------------------

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution.xy;
    
    //vec2 med = vec2(0.5,0.5);//(uv*0.5) - 1.0;
    vec2 med = vec2(0.5);
    med.x += cos(uTime) * sin(uTime * 0.1) * 0.75;
    med.y += sin(uTime) * 0.5;
    
    vec2 disVec = med - uv;
    //disVec += vec2(cos(disVec.x + iTime), sin(disVec.y + iTime));
    float l = length(disVec);
    float ll = 1.0 - l*l;
    vec2 dist = med - disVec*ll;
    
    fragColor = texture(uBase, dist);
}
