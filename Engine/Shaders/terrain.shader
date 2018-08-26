#shader vertex

#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColour;
layout(location = 3) in vec3 aTexCoords;

out vec3 g_FragPos;
out vec3 g_Normal;
out vec3 g_TexCoords;
out vec3 g_Colour;

uniform mat4 u_ModelMatrix;

void main()
{
	g_FragPos = vec3(u_ModelMatrix * vec4(aPos, 1.0));
	g_Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;
	g_TexCoords = aTexCoords;
	g_Colour = aColour;

	gl_Position = u_ModelMatrix * vec4(aPos, 1.0);

}

#shader geometry

#version 410

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 u_ViewMatrix[4];
uniform mat4 u_ProjectionMatrix[4];

//in vec3 g_FragPos;
//in vec3 g_Normal[4];
//in vec3 g_TexCoords[4];
//in vec3 g_Colour[4];
//
//out vec3 FragPos[4];
//out vec3 Normal[4];
//out vec3 TexCoords[4];
//out vec3 Colour[4];

void main() {

	// for every vertex in the triangle...
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_ViewportIndex = gl_InvocationID;
		// add here any other viewport-specific transform
		gl_Position = u_ProjectionMatrix[0] * u_ViewMatrix[0] * gl_in[i].gl_Position;

		/*FragPos[gl_InvocationID] = g_FragPos;
		Normal[gl_InvocationID] = g_Normal[gl_InvocationID];
		TexCoords[gl_InvocationID] = g_TexCoords[gl_InvocationID];
		Colour[gl_InvocationID] = g_Colour[gl_InvocationID];*/

		EmitVertex();
	}
	
	EndPrimitive();
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