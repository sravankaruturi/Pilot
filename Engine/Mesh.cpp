#include "Mesh.h"
#include "Texture.h"
#include "GLShader.h"

namespace piolot
{

	class GLShader;

	//template <typename VertexDataTemplate>
	//Mesh::Mesh(std::vector<VertexDataTemplate> _vertices, std::vector<unsigned> _indices)
	//{

	//	usingIndexBuffer = true;
	//	indexCount = _indices.size();
	//	vertexCount = _vertices.size();

	//	glGenVertexArrays(1, &VAO);
	//	glGenBuffers(1, &VBO);
	//	glGenBuffers(1, &EBO);

	//	glBindVertexArray(VAO);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDataTemplate) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_FALSE);

	//	for (auto i = 0; i < sizeof(VertexDataTemplate) / 3 * sizeof(float); i++)
	//	{
	//		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataTemplate), (void *)(3 * i * sizeof(float)));
	//		glEnableVertexAttribArray(i);
	//	}

	//}

	void Mesh::Render(GLShader* _shader, std::vector<Texture*> _textures) const
	{

		_shader->use();

		auto i = 0;
		for ( const auto it : _textures)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, it->GetTextureId());
			_shader->setInt("texture" + std::to_string(i), i);
			i++;
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if ( usingIndexBuffer )
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		}else
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}

	Mesh::Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount)
	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		usingIndexBuffer = false;
		EBO = 0;
		indexCount = 0;

		vertexCount = _vertexCount;

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW);

		for (auto i = 0; i < (_dataStructureSize) / (3 * sizeof(float)); i++)
		{
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, (_dataStructureSize), (void *)(3 * i * sizeof(float)));
			glEnableVertexAttribArray(i);
			vertexAttribCounter += 1;
		}

	}

	Mesh::Mesh(void* _dataPointer, size_t _dataStructureSize, unsigned _vertexCount, std::vector<unsigned> _indices)
	{
		usingIndexBuffer = true;
		indexCount = _indices.size();
		vertexCount = _vertexCount;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_FALSE);

		for (auto i = 0; i < _dataStructureSize / (3 * sizeof(float)); i++)
		{
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, _dataStructureSize, (void *)(3 * i * sizeof(float)));
			glEnableVertexAttribArray(i);
			vertexAttribCounter += 1;
		}
	}
}
