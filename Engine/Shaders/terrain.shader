#shader vertex

#version 430 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec4 aTexCoords;

struct vData {
	vec3 g_FragPos;
	vec3 g_Normal;
	vec3 g_TexCoords;
	vec3 g_Colour;
};

out vData g_Stuff;

uniform mat4 u_ModelMatrix;

void main()
{

	g_Stuff.g_FragPos = vec3(u_ModelMatrix * vec4(aPos.xyz, 1.0));
	g_Stuff.g_Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal.xyz;
	g_Stuff.g_TexCoords = aTexCoords.xyz;
	g_Stuff.g_Colour = aColour.xyz;

	gl_Position = u_ModelMatrix * vec4(aPos.xyz, 1.0);

}

#shader geometry

#version 430 core

struct vData {
	vec3 g_FragPos;
	vec3 g_Normal;
	vec3 g_TexCoords;
	vec3 g_Colour;
};

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 u_ViewMatrix[4];
uniform mat4 u_ProjectionMatrix[4];

in vData g_Stuff[];
out vData f_Stuff;

void main() {

	// for every vertex in the triangle...
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_ViewportIndex = gl_InvocationID;

		// add here any other viewport-specific transform
		gl_Position = u_ProjectionMatrix[gl_InvocationID] * u_ViewMatrix[gl_InvocationID] * gl_in[i].gl_Position;

		f_Stuff = g_Stuff[i];

		EmitVertex();
	}
	
	//EndPrimitive();
}


#shader fragment

#version 430 core

struct vData {
	vec3 g_FragPos;
	vec3 g_Normal;
	vec3 g_TexCoords;
	vec3 g_Colour;
};

uniform sampler2D u_Texture0;

out vec4 FragColour;

in vData f_Stuff;

void main() {

	FragColour = mix(vec4(texture(u_Texture0, f_Stuff.g_TexCoords.xy)), vec4(f_Stuff.g_Colour, 1), f_Stuff.g_TexCoords.z);

}