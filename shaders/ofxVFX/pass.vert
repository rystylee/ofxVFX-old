#version 400

in vec4 color; // oF default
in vec4 position; // oF default
in vec4 normal; // oF default
in vec2 texcoord; // oF default

out vec4 vColor;
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 modelViewProjectionMatrix; // oF default
uniform mat4 modelViewMatrix; // oF default

void main()
{
    vColor = color;
    vPosition = position;
    vNormal = normal;
    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
