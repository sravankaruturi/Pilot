#shader vertex
#version 430 core

layout(location = 0) in vec4 aPos;

uniform mat4 u_ModelMatrix;

void main() {
	gl_Position = u_ModelMatrix * vec4(aPos.xyz, 1.0);
}

#shader geometry
#version 430 core

layout(lines, invocations = 4) in;
layout(line_strip, max_vertices = 10) out;

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

}

#shader fragment
#version 430 core

uniform vec3 u_Colour;

out vec4 FragColour;

void main() {
	FragColour = vec4(u_Colour, 1.0);
}