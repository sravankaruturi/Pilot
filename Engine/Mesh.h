#pragma once
#include <vector>
#include <glad/glad.h>
#include <memory>

namespace piolot
{

	class GLShader;
	class Texture;
	class AssetManager;

	// Mesh is part of an Object. A single Mesh is typically an arm or the head of the Object.
	class Mesh
	{

	protected:

		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;

		// For Debugging Purposes.
		unsigned int vertexAttribCounter = 0;

		std::vector<std::string> textureNames;

		// Just because it is expesive to look up textures from the names every frame.
		std::vector<std::shared_ptr<Texture>> texturePointers;

	public:
		const std::vector<std::string>& GetTextureNames() const
		{
			return textureNames;
		}

		void SetTextureNames(const std::vector<std::string>& _basicStrings)
		{
			textureNames = _basicStrings;
		}

		unsigned GetVertexAttribCounter() const
		{
			return vertexAttribCounter;
		}

		unsigned GetVbo() const
		{
			return VBO;
		}

		unsigned GetVao() const
		{
			return VAO;
		}

		unsigned GetEbo() const
		{
			return EBO;
		}

		bool IsUsingIndexBuffer() const
		{
			return usingIndexBuffer;
		}

		unsigned GetVertexCount() const
		{
			return vertexCount;
		}

		unsigned GetIndexCount() const
		{
			return indexCount;
		}

	protected:
		bool usingIndexBuffer = true;

		// We do not store the Indices or Vertices in the Object.

		unsigned int vertexCount;
		unsigned int indexCount;

		unsigned int openglErrorFlag;

	public:
		unsigned int GetOpenglErrorFlag() const
		{
			return openglErrorFlag;
		}

		// A Couple of rules for the DataStructure. All the attributes should be size of 3. You have to pack them like that.
		//explicit Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount);

		explicit Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount, std::vector<unsigned int> _indices = std::vector<unsigned int>());

		// Use this to update the vertex data.. once in a while. Not alywas as this is expensive.
		void UpdateVertices(void* _dataPointer, size_t _dataStructureSize, unsigned _vertexCount);

		void Render(const std::string& _shaderName);
	};

}