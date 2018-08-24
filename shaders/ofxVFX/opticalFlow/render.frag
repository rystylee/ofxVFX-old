#version 400 core
precision highp float;

uniform sampler2DRect uFlow;
uniform sampler2DRect tex;
uniform vec2 uResolution;
uniform float uAmt;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
//    vec2 uv = vPosition.xy;
    vec2 uv = vTexCoord.xy;
    
    vec2 amt = uResolution * uAmt;
    vec2 flow = texture(uFlow, uv).rg; // sample flowBuffer
    vec2 offset = vec2(flow.x, flow.y) * amt;
    vec2 coord = offset + uv;
    
//    if(length(flow.rg) < 0.1)
//    {
//        fragColor = texture(uCam, uv); // sample currentBuffer
//    } else
//    {
//        fragColor = texture(uCam, coord); // sample currentBuffer
//    }
    
    fragColor = texture(tex, coord);
}
