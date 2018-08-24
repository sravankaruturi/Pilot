#shader vertex

#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColour;
layout(location = 3) in vec3 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 TexCoords;
out vec3 Colour;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	FragPos = vec3(u_ModelMatrix * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;
	TexCoords = aTexCoords;
	Colour = aColour;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(FragPos, 1.0);
}

#shader fragment

#version 410

out vec4 FragColour;

uniform sampler2D u_Texture0;

in vec3 FragPos;
in vec3 Normal;
in vec3 TexCoords; // The last portion of the TexCoords, the z value is if the Colour should be used?
in vec3 Colour;

void main() {

	FragColour = mix(vec4(texture(u_Texture0, TexCoords.xy)), vec4(Colour, 1.0), TexCoords.z);

}