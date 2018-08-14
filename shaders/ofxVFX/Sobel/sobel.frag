#version 400
precision mediump float;

in vec4 vPosition;

out vec4 fragColor;

uniform sampler2DRect uBase;

// Use these parameters to fiddle with settings
float step = 1.0;

float intensity(in vec4 color)
{
    return sqrt((color.x*color.x)+(color.y*color.y)+(color.z*color.z));
}

vec3 sobel(float stepx, float stepy, vec2 uv){
    // get samples around pixel
    float tleft = intensity(texture(uBase, uv + vec2(-stepx, stepy)));
    float left = intensity(texture(uBase, uv + vec2(-stepx, 0)));
    float bleft = intensity(texture(uBase, uv + vec2(-stepx, -stepy)));
    float top = intensity(texture(uBase, uv + vec2(0, stepy)));
    float bottom = intensity(texture(uBase, uv + vec2(0, -stepy)));
    float tright = intensity(texture(uBase, uv + vec2(stepx, stepy)));
    float right = intensity(texture(uBase, uv + vec2(stepx, 0)));
    float bright = intensity(texture(uBase, uv + vec2(stepx, -stepy)));
    
    // Sobel masks (see http://en.wikipedia.org/wiki/Sobel_operator)
    //        1 0 -1     -1 -2 -1
    //    X = 2 0 -2  Y = 0  0  0
    //        1 0 -1      1  2  1
    
    // You could also use Scharr operator:
    //        3 0 -3        3 10   3
    //    X = 10 0 -10  Y = 0  0   0
    //        3 0 -3        -3 -10 -3
    
    float x = tleft + 2.0*left + bleft - tright - 2.0*right - bright;
    float y = -tleft - 2.0*top - tright + bleft + 2.0 * bottom + bright;
    float color = sqrt((x*x) + (y*y));
    return vec3(color,color,color);
}

void main()
{
    vec2 uv = vPosition.xy;
    
//    vec2 uv = fragCoord.xy / iResolution.xy;
//    vec4 color = texture(iChannel0, uv.xy);
    vec3 color = sobel(step, step, uv);
    
//    vec4 color = texture(uBase, uv);
    fragColor = vec4(color, 1.0);
//    fragColor = vec4(1.0);
}
