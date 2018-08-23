#version 400 core
precision highp float;

uniform sampler2DRect uBlur;
uniform sampler2DRect tex;
uniform vec2 uResolution;
uniform float uAmt;
uniform float uScale;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

// ------------------------------------------------------------------------
// reference : https://www.interactiveshaderformat.com/sketches/227
// ------------------------------------------------------------------------

vec4 neighbor(in sampler2DRect tex, in vec2 uv)
{
    float offset = (8.0 / uResolution.x) * uScale;
    vec4 sum = vec4(0);
    
    sum += texture(tex, uv);
    sum += texture(tex, uv + vec2(offset, 0));
    sum += texture(tex, uv + vec2(-offset, 0));
    sum += texture(tex, uv + vec2(0.0, offset));
    sum += texture(tex, uv + vec2(0.0, -offset));
    sum += texture(tex, uv + vec2(offset, offset));
    sum += texture(tex, uv + vec2(-offset, offset));
    sum += texture(tex, uv + vec2(offset, -offset));
    sum += texture(tex, uv + vec2(-offset, -offset));
    
    return sum / 9.0;
}

void main()
{
    vec2 uv = vTexCoord.xy;
    vec2 UV = uv / uResolution;
//
//    vec2 flowBlur = texture(uBlur, uv).xy;
//    vec4 base = texture(tex, uv);
//    vec4 flowBase = texture(tex, uv + scale * flowBlur);
//
//    UV += scale * flowBlur;
//    vec4 col = texture(tex, UV * uResolution);
//
////    fragColor = col;
//    fragColor = vec4((vec3(base.rgb + col.rgb * 3.0) / 4.0), base.a);
    
    vec4 blurBase = neighbor(tex, uv);

    //    vec4 blurFlow = neighbor(uBlur, uv);
    vec4 blurFlow = neighbor(uBlur, uv);
    //    blurFlow = vec4(-1 * (blurFlow.y - blurFlow.x), 0.0, 0.0, 1.0);
    vec2 flowBlur = vec2(-1 * (blurFlow.y - blurFlow.x), 0.0);
    vec4 col1 = texture(tex, vec2(UV +  flowBlur * uAmt) * uResolution);
    vec4 col2 = texture(tex, vec2(UV + flowBlur * uAmt * uAmt) * uResolution);
    fragColor = (col1 + col2 * 3.0) / 4;
}
