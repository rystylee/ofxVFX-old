#version 410
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;
uniform int uDirection; // 0 == verticel, 1 == horizontal
uniform float uPixelSize;

void main()
{
    vec2 uv = vPosition.xy;
    
    vec2 cor = vec2(0);
    if(uDirection == 0)
    {
        cor.x = uv.x / uPixelSize;
        cor.y = (uv.y + uPixelSize * 1.5 * mod(floor(cor.x), 2.0)) / (uPixelSize * 3.0);
    }
    else
    {
        cor.y = uv.y / uPixelSize;
        cor.x = (uv.x + uPixelSize * 1.5 * mod(floor(cor.y), 2.0)) / (uPixelSize * 3.0);
    }
    
    vec2 ico = floor(cor);
    vec2 fco = fract(cor);
    
    vec3 pix = vec3(0);
    vec3 ima = vec3(0);
    if(uDirection == 0)
    {
        pix = step(1.5, mod(vec3(0.0, 1.0, 2.0) + ico.x, 3.0));
        ima = texture(uBase, uPixelSize * ico * vec2(1.0, 3.0)).xyz;
    }
    else
    {
        pix = step(1.5, mod(vec3(0.0, 1.0, 2.0) + ico.y, 3.0));
        ima = texture(uBase, uPixelSize * ico * vec2(3.0, 1.0)).xyz;
    }
    
    vec3 col = pix * dot(pix, ima);
    
    col *= step(abs(fco.x - 0.5), 0.4);
    col *= step(abs(fco.y - 0.5), 0.4);
    
    col *= 1.2;
    fragColor = vec4(col, 1.0);
}
