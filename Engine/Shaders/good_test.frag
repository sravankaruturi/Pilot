#version 410 core
out vec4 FragColour;

in vec2 texCoord;

uniform sampler2D u_Texture0;

void main()
{
    FragColour = texture(u_Texture0, texCoord);
}