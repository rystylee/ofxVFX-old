#version 400 core
precision highp float;

uniform sampler2DRect uBackBuffer;
uniform sampler2DRect tex;
uniform float uThresh;
uniform float uOffset;
uniform float uLambda;

in vec4 vPosition;
in vec2 vTexCoord;

out vec4 fragColor;

vec4 getGrayScale(vec4 col)
{
	float gray = dot(vec3(col.x, col.y, col.z), vec3(0.3, 0.59, 0.11));
	return vec4(gray, gray, gray, 1);
}

vec4 textureGray(sampler2DRect tex, vec2 texCoord)
{
    return getGrayScale(texture(tex, texCoord));
}

vec4 gradient(sampler2DRect tex, vec2 uv, vec2 offset)
{
//    return vec4(textureGray(tex, uv + offset) - textureGray(tex, uv - offset));
    return vec4(texture(tex, uv + offset) - texture(tex, uv - offset));
}


void main()
{
    vec2 uv = vTexCoord.xy;
    vec4 prev = texture(uBackBuffer, uv);
    vec4 current = texture(tex, uv);
    vec4 diff = current - prev;

    vec2 offX = vec2(uOffset, 0.0);
    vec2 offY = vec2(0.0, uOffset);

    // calculate the gradient
    vec4 gradX = gradient(uBackBuffer, uv, offX) + gradient(tex, uv, offX);
    vec4 gradY = gradient(uBackBuffer, uv, offY) + gradient(tex, uv, offY);

    vec4 gradMag = sqrt((gradX * gradX) + (gradY * gradY) + vec4(uLambda));
    vec4 invGMag = 1.0 / gradMag;
    
    vec4 vx = diff * (gradX * invGMag);
    vec4 vy = diff * (gradY * invGMag);

    vec2 flow = vec2(0.0);
    flow.x = -(vx.x + vx.y + vx.z) / 3.0;
    flow.y = -(vy.x + vy.y + vy.z) / 3.0;
    
    float strength = length(flow);
    if (strength * uThresh > 0.0)
    {
        if (strength < uThresh)
        {
            flow = vec2(0.0);
        }
        else
        {
            strength = (strength - uThresh) / (1.0 - uThresh);
            flow = normalize(flow) * vec2(strength);
        }
    }

    fragColor = vec4(flow, 0.0, 1.0);
}
