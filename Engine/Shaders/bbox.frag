#version 410 core
out vec4 FragColour;

in vec2 TexCoord;

uniform vec3 u_Colour;

void main()
{
    FragColour = vec4(u_Colour, 1.0);
}