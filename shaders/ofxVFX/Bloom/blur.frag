#version 410
precision mediump float;

in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2DRect tex;
uniform int uDirection;
uniform float uOffsetScale;
uniform float uAttenuation;

void main()
{
    vec2 uv = vTexCoord.xy;
    
    vec2 sampleOffset;
    if(uDirection == 0)  // horizontal
        sampleOffset = vec2(1.0, 0.0);
    else  // vertical
        sampleOffset = vec2(0.0, 1.0);
    sampleOffset *= uOffsetScale;
    
    vec3 sum = vec3(0.0, 0.0, 0.0);
    sum += texture(tex, uv + -10.0 * sampleOffset).rgb * 0.009167927656011385;
    sum += texture(tex, uv +  -9.0 * sampleOffset).rgb * 0.014053461291849008;
    sum += texture(tex, uv +  -8.0 * sampleOffset).rgb * 0.020595286319257878;
    sum += texture(tex, uv +  -7.0 * sampleOffset).rgb * 0.028855245532226279;
    sum += texture(tex, uv +  -6.0 * sampleOffset).rgb * 0.038650411513543079;
    sum += texture(tex, uv +  -5.0 * sampleOffset).rgb * 0.049494378859311142;
    sum += texture(tex, uv +  -4.0 * sampleOffset).rgb * 0.060594058578763078;
    sum += texture(tex, uv +  -3.0 * sampleOffset).rgb * 0.070921288047096992;
    sum += texture(tex, uv +  -2.0 * sampleOffset).rgb * 0.079358891804948081;
    sum += texture(tex, uv +  -1.0 * sampleOffset).rgb * 0.084895951965930902;
    sum += texture(tex, uv +   0.0 * sampleOffset).rgb * 0.086826196862124602;
    sum += texture(tex, uv +  +1.0 * sampleOffset).rgb * 0.084895951965930902;
    sum += texture(tex, uv +  +2.0 * sampleOffset).rgb * 0.079358891804948081;
    sum += texture(tex, uv +  +3.0 * sampleOffset).rgb * 0.070921288047096992;
    sum += texture(tex, uv +  +4.0 * sampleOffset).rgb * 0.060594058578763078;
    sum += texture(tex, uv +  +5.0 * sampleOffset).rgb * 0.049494378859311142;
    sum += texture(tex, uv +  +6.0 * sampleOffset).rgb * 0.038650411513543079;
    sum += texture(tex, uv +  +7.0 * sampleOffset).rgb * 0.028855245532226279;
    sum += texture(tex, uv +  +8.0 * sampleOffset).rgb * 0.020595286319257878;
    sum += texture(tex, uv +  +9.0 * sampleOffset).rgb * 0.014053461291849008;
    sum += texture(tex, uv + +10.0 * sampleOffset).rgb * 0.009167927656011385;
    
    sum *= uAttenuation;
    fragColor = vec4(sum, 1.0); 
}
