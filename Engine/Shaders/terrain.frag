#version 330

out vec4 FragColour;

uniform sampler2D u_Texture0;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Colour;

void main(){

	// FragColour = mix( vec4(texture(u_Texture0, TexCoords)), vec4(Colour, 1.0), 0.15);
	// FragColour = texture(u_Texture0, TexCoords);
	FragColour = vec4(Colour, 1.0);

}