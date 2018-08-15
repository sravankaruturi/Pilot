#version 330 core
out vec4 FragColour;

uniform vec3 u_Colour;

void main()
{
    FragColour = vec4(u_Colour, 1.0);
	FragColour = vec4(1.0, 1.0, 1.0, 1.0);
}