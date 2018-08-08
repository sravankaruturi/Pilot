#version 330 core
out vec4 FragColour;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

void main()
{
    FragColour = texture(texture0, TexCoord);
}