#version 400

in vec4 color; // oF default
in vec4 position; // oF default
in vec4 normal; // oF default
in vec2 texcoord; // oF default

out vec4 vColor;
out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 modelViewProjectionMatrix; // oF default
uniform mat4 modelViewMatrix; // oF default

void main()
{
    vTexCoord = texcoord;
    vNormal = normal.xyz;
    vColor = color;
    gl_Position = modelViewProjectionMatrix * position;
}
