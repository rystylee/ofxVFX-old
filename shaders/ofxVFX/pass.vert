#version 410

// -----------------------------
// oF default
// -----------------------------
in vec4 color;
in vec4 position;
in vec4 normal;
in vec2 texcoord;

out vec4 vColor;
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

// -----------------------------
// oF default
// -----------------------------
uniform mat4 modelViewProjectionMatrix;

void main()
{
    vColor = color;
    vPosition = position;
    vNormal = normal;
    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
