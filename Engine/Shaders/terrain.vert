#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColour;
layout (location = 3) in vec3 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Colour;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    FragPos = vec3(u_ModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;  
    TexCoords = vec2(aTexCoords.xy);
    Colour = aColour;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(FragPos, 1.0);
}