#pragma once
#include <vector>
#include <glad/glad.h>

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

	public:
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
		explicit Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount);

		Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount, std::vector<unsigned int> _indices);

		void Render(GLShader * _shader, std::vector<Texture *> _textures);
	};

}
