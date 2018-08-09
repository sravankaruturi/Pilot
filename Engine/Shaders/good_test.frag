#version 330 core
out vec4 FragColour;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
    FragColour = (texture(texture0, texCoord) + vec4(ourColor, 1.0))/2;
}