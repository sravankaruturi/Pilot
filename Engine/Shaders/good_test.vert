#version 410 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormals;
layout (location = 2) in vec4 aTexCoord;

out vec2 texCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPos.xyz, 1.0);
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
}