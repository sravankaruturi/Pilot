#shader vertex

#version 430 core

const int MAX_BONES = 128;

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec4 aTexCoords;
layout(location = 3) in ivec4 aBoneIds;
layout(location = 4) in ivec4 aBoneIds2;
layout(location = 5) in vec4 aWeights;
layout(location = 6) in vec4 aWeights2;

struct vData {
	vec3 g_FragPos;
	vec3 g_Normal;
	vec2 g_TexCoords;
	vec3 g_Colour;
};

out vData g_Stuff;

uniform mat4 u_ModelMatrix;

uniform mat4 u_BoneMatrices[MAX_BONES];

void main()
{

	g_Stuff.g_FragPos = vec3(u_ModelMatrix * vec4(aPos.xyz, 1.0));
	g_Stuff.g_Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal.xyz;
	g_Stuff.g_TexCoords = aTexCoords.xy;

	mat4 boneTransforms = u_BoneMatrices[aBoneIds[0]] * aWeights[0];
	boneTransforms += u_BoneMatrices[aBoneIds[1]] * aWeights[1];
	boneTransforms += u_BoneMatrices[aBoneIds[2]] * aWeights[2];
	boneTransforms += u_BoneMatrices[aBoneIds[3]] * aWeights[3];
	boneTransforms += u_BoneMatrices[aBoneIds2[0]] * aWeights2[0];
	boneTransforms += u_BoneMatrices[aBoneIds2[1]] * aWeights2[1];
	boneTransforms += u_BoneMatrices[aBoneIds2[2]] * aWeights2[2];
	boneTransforms += u_BoneMatrices[aBoneIds2[3]] * aWeights2[3];

	gl_Position = u_ModelMatrix * boneTransforms * vec4(aPos.xyz, 1.0);
	g_Stuff.g_Colour = vec3(1, 0, 0);

}

#shader geometry

#version 430 core

struct vData {
	vec3 g_FragPos;
	vec3 g_Normal;
	vec2 g_TexCoords;
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
	vec2 g_TexCoords;
	vec3 g_Colour;
};

uniform sampler2D u_Texture0;

out vec4 FragColour;

in vData f_Stuff;

void main() {

	/*FragColour = mix(vec4(texture(u_Texture0, f_Stuff.g_TexCoords.xy)), vec4(f_Stuff.g_Colour, 1.0), f_Stuff.g_TexCoords.z);*/
	FragColour = mix(texture(u_Texture0, f_Stuff.g_TexCoords), vec4(f_Stuff.g_Colour, 1.0), 0.1);

}