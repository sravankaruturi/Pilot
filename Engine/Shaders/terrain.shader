#shader vertex

#version 430 core
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
	gl_Position = u_ModelMatrix * vec4(aPos, 1.0);

}

#shader geometry

#version 430 core

layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 u_ViewMatrix[4];
uniform mat4 u_ProjectionMatrix[4];

void main() {

	// for every vertex in the triangle...
	for (int i = 0; i < gl_in.length(); i++)
	{
		gl_ViewportIndex = gl_InvocationID;

		// add here any other viewport-specific transform
		gl_Position = u_ProjectionMatrix[gl_InvocationID] * u_ViewMatrix[gl_InvocationID] * gl_in[i].gl_Position;

		EmitVertex();
	}
	
	//EndPrimitive();
}


#shader fragment

#version 430 core

out vec4 FragColour;

void main() {

	FragColour = vec4(0.1, 0.9, 0.1, 1.0);

}