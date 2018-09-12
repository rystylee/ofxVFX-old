#version 410
precision highp float;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2DRect uSimuTex;
uniform sampler2DRect uBase;
uniform vec2 uResolution;
uniform int uIsOnlyMNCA;
uniform int uColorMode;
uniform float uTime;

float col(sampler2DRect tex, vec2 uv)
{
    float c1 = texture(tex, uv + vec2(-1, 0)).r;
    float c2 = texture(tex, uv + vec2(1, 0)).r;
    float c3 = texture(tex, uv + vec2(0, -1)).r;
    float c4 = texture(tex, uv + vec2(0, 1)).r;
    return (c1 + c2 + c3 + c4) / 4;
}

void main()
{
    vec2 uv = vPosition.xy;

    vec3 result = texture(uSimuTex, uv).rgb;
    float body = result.r;
    float blob = result.g;
    float lineblob = result.b;
    // r = result
    // g = blob and result
    // b = line blob
    
    vec3 base = texture(uBase, uv).rgb;
    vec3 color = vec3(0);
    
    int uIsOnlyMNCA = 1;

    if(uIsOnlyMNCA == 0)
    {
        // mnca + base color
        if(uColorMode == 0) // White
            color = base + vec3(result.b);
        if(uColorMode == 1) // Red and Yellow
            color = base + vec3(result.g, result.r, result.b*0.15);
        else if(uColorMode == 2) // Red and Pink
            color = base + vec3(result.b, result.r, result.g);
        else if(uColorMode == 3) // Blue and Green
            color = base + vec3(result.b*0.175, result.r, result.g);
        else
            color = base + vec3(result.g);
    }
    else
    {
        // mnca color only
        if(uColorMode == 0) // White
            color = vec3(result.b);
        if(uColorMode == 1) // Red and Yellow
            color = vec3(result.g, result.r, result.b*0.15);
        else if(uColorMode == 2) // Red and Pink
            color = vec3(result.b, result.r, result.g);
        else if(uColorMode == 3) // Blue and Green
            color = vec3(result.b*0.175, result.r, result.g);
        else
            color = base + vec3(result.g);
    }
    
    fragColor = vec4(color, 1.0);
}
