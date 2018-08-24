#version 410

out vec4 FragColour;

uniform sampler2D u_Texture0;

in vec3 FragPos;
in vec3 Normal;
in vec3 TexCoords; // The last portion of the TexCoords, the z value is if the Colour should be used?
in vec3 Colour;	

void main(){

	FragColour = mix( vec4(texture(u_Texture0, TexCoords.xy)), vec4(Colour, 1.0), TexCoords.z);

}