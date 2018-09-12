#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform int uDirection; // 0 == verticel, 1 == horizon1tal

void main()
{
    vec2 uv = vPosition.xy;
    
    // to -1.0 ~ 1.0
    vec2 UV = (uv.xy * 2.0 - uResolution.xy) / min(uResolution.x, uResolution.y);
    
    if(uDirection == 0)
    {
        if(UV.x > 0.0) UV.x = - UV.x;
    }
    else
    {
        if (UV.y > 0.0) UV.y = - UV.y;
    }
    
    // to 0 ~ uResolution
    UV = (min(uResolution.x, uResolution.y) * UV + uResolution) * 0.5;
    
    vec4 col = texture(uBase, UV);
    fragColor = col;
}
