#include "Mesh.h"
#include "Texture.h"
#include "GLShader.h"

namespace piolot
{

	class GLShader;

	void Mesh::Render(GLShader* _shader, std::vector<Texture*> _textures)
	{

		_shader->use();

		auto i = 0;
		for ( const auto it : _textures)
		{
			PE_GL(glActiveTexture(GL_TEXTURE0 + i));
			PE_GL(glBindTexture(GL_TEXTURE_2D, it->GetTextureId()));
			_shader->setInt("texture" + std::to_string(i), i);
			i++;
		}

		PE_GL(glBindVertexArray(VAO));
		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

		if ( usingIndexBuffer )
		{
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			PE_GL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
		}else
		{
			PE_GL(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
		}
	}

	Mesh::Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount)
	{

		PE_GL(glGenVertexArrays(1, &VAO));
		PE_GL(glGenBuffers(1, &VBO));

		usingIndexBuffer = false;
		EBO = 0;
		indexCount = 0;

		vertexCount = _vertexCount;

		PE_GL(glBindVertexArray(VAO));

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW));

		for (auto i = 0; i < (_dataStructureSize) / (3 * sizeof(float)); i++)
		{
			PE_GL(glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, (_dataStructureSize), (void *)(3 * i * sizeof(float))));
			PE_GL(glEnableVertexAttribArray(i));
			vertexAttribCounter += 1;
		}
	}

	Mesh::Mesh(void* _dataPointer, size_t _dataStructureSize, unsigned _vertexCount, std::vector<unsigned> _indices)
	{

		usingIndexBuffer = true;
		indexCount = _indices.size();
		vertexCount = _vertexCount;

		PE_GL(glGenVertexArrays(1, &VAO));
		PE_GL(glGenBuffers(1, &VBO));
		PE_GL(glGenBuffers(1, &EBO));

		PE_GL(glBindVertexArray(VAO));

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW));

		PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		PE_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW));

		for (auto i = 0; i < _dataStructureSize / (3 * sizeof(float)); i++)
		{
			PE_GL(glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, _dataStructureSize, (void *)(3 * i * sizeof(float))));
			PE_GL(glEnableVertexAttribArray(i));
			vertexAttribCounter += 1;
		}
	}
}
