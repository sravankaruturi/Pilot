#version 330 core
out vec4 FragColour;

in vec2 texCoord;

uniform sampler2D texture0;

void main()
{
    FragColour = texture(texture0, texCoord);
}