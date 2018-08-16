#version 400
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform vec2 uResolution;

// 0 == verticel, 1 == horizon1tal
const int direction = 1;

// ------------------------------------------------------
// https://www.shadertoy.com/view/Xdf3Rf
// ------------------------------------------------------

void main()
{
    vec2 uv = vPosition.xy;
    
    // to -1.0 ~ 1.0
    vec2 p = (uv.xy * 2.0 - uResolution.xy) / min(uResolution.x, uResolution.y);
    
    if(direction == 0)
    {
        if (p.x > 0.0) p.x = - p.x;
    }
    else
    {
        if (p.y > 0.0) p.y = - p.y;
    }
    
    // to 0 ~ uResolution
    vec2 newTexCoord = (min(uResolution.x, uResolution.y) * p + uResolution) * 0.5;
    
    vec4 col = texture(uBase, newTexCoord);
    
    fragColor = col;
}
