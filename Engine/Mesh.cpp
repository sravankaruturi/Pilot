#include "Mesh.h"
#include "Texture.h"
#include "GLShader.h"
#include "AssetManager.h"

namespace piolot
{

	// This assumes, and is hardcoded to make sure that all the values passed in are vec4s. Pad stuff if you need smaller vectors.
	const int size_of_ints_per_input_vec = 4;

	void Mesh::Render(const std::string& _shaderName)
	{
		if (textureNames.size() != texturePointers.size())
		{
			texturePointers.clear();
			for (auto it : textureNames)
			{
				texturePointers.push_back(ASMGR.textures.at(it));
			}
		}

		ASMGR.shaders.at(_shaderName)->use();

		auto i = 0;
		for (const auto it : texturePointers)
		{
			PE_GL(glActiveTexture(GL_TEXTURE0 + i));
			PE_GL(glBindTexture(GL_TEXTURE_2D, it->GetTextureId()));
			ASMGR.shaders.at(_shaderName)->setInt("u_Texture" + std::to_string(i), i);
			i++;
		}

		PE_GL(glBindVertexArray(this->VAO));
		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));

		if (usingIndexBuffer)
		{
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			PE_GL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
			PE_GL(glBindVertexArray(0));
		}
		else
		{
			PE_GL(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
			PE_GL(glBindVertexArray(0));
		}
	}

	//Mesh::Mesh(void * _dataPointer, size_t _dataStructureSize, unsigned int _vertexCount)
	//{

	//	PE_GL(glGenVertexArrays(1, &VAO));
	//	PE_GL(glGenBuffers(1, &VBO));

	//	usingIndexBuffer = false;
	//	EBO = 0;
	//	indexCount = 0;

	//	vertexCount = _vertexCount;

	//	// Read and remove the header first.
	//	short header_read[8] = { 0 };
	//	int short_size = sizeof(short);

	//	// Read the short.
	//	short my_header = 0;
	//	memcpy(&my_header, _dataPointer, short_size);

	//	// Split it to the header_read array.
	//	short divisor = 1;
	//	for ( int i = 0 ; i < 8 ; i++)
	//	{
	//		header_read[8 - i - 1] = my_header / divisor;
	//		divisor *= 10;
	//	}

	//	// We remove the header from the data, so that we do not accidentally pass it.
	//	// To Increment the Pointer, we should cast it to something. Incrementing Void * is not allowed.
	//	// @see https://stackoverflow.com/questions/24472724/expression-must-be-a-pointer-to-a-complete-object-type-using-simple-pointer-arit
	//	short * temp_pointer = (short *)_dataPointer;
	//	temp_pointer++;
	//	_dataPointer = (void*)temp_pointer;

	//	PE_GL(glBindVertexArray(VAO));

	//	PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	//	PE_GL(glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW));

	//	for (auto i = 0; i < (_dataStructureSize) / (size_of_ints_per_input_vec * sizeof(float)); i++)
	//	{
	//		PE_GL(glVertexAttribPointer(i, size_of_ints_per_input_vec, GL_FLOAT, GL_FALSE, (_dataStructureSize), (void *)(size_of_ints_per_input_vec * i * sizeof(float))));
	//		PE_GL(glEnableVertexAttribArray(i));
	//		vertexAttribCounter += 1;
	//	}
	//}

	Mesh::Mesh(void* _dataPointer, size_t _dataStructureSize, unsigned _vertexCount, std::vector<unsigned> _indices)
	{

		if (_indices.empty())
		{
			usingIndexBuffer = false;
		}
		else
		{
			usingIndexBuffer = true;
		}

		indexCount = _indices.size();
		vertexCount = _vertexCount;

		// Read and remove the header first.
		short header_read[8] = { 0 };
		int long_size = sizeof(long);

		// Read the long int.
		long my_header = 0;
		memcpy(&my_header, _dataPointer, long_size);

		// Split it to the header_read array.
		long divisor = 1;
		for (int i = 0; i < 8; i++)
		{
			header_read[8 - i - 1] = ( my_header / divisor ) % 10;
			divisor *= 10;
		}

		// We remove the header from the data, so that we do not accidentally pass it.
		// To Increment the Pointer, we should cast it to something. Incrementing Void * is not allowed.
		// @see https://stackoverflow.com/questions/24472724/expression-must-be-a-pointer-to-a-complete-object-type-using-simple-pointer-arit
		long * temp_pointer = (long *)_dataPointer;
		temp_pointer++;
		_dataPointer = (void*)temp_pointer;

		_dataStructureSize = _dataStructureSize - sizeof(long);


		PE_GL(glGenVertexArrays(1, &VAO));
		PE_GL(glGenBuffers(1, &VBO));

		if (usingIndexBuffer)
		{
			PE_GL(glGenBuffers(1, &EBO));
		}

		PE_GL(glBindVertexArray(VAO));

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW));

		if (usingIndexBuffer)
		{
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			PE_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW));
		}

		// This assumes, and is hardcoded to make sure that all the values passed in are vec4s. Pad stuff if you need smaller vectors.
		int offset = 0;
		for (auto i = 0; i < sizeof(header_read)/sizeof(short); i++)
		{

			if( header_read[i] == 0)
			{
				break;
			}

			if ( header_read[i] == 1)
			{
				PE_GL(glVertexAttribPointer(i, size_of_ints_per_input_vec, GL_FLOAT, GL_FALSE, _dataStructureSize, (void *)(offset)));
				offset += (4 * sizeof(float));
			}else if (header_read[i] == 2)
			{
				PE_GL(glVertexAttribIPointer(i, size_of_ints_per_input_vec, GL_INT, _dataStructureSize, (void *)(offset)));
				offset += (4 * sizeof(int));
			}
			
			PE_GL(glEnableVertexAttribArray(i));
			vertexAttribCounter += 1;
		}
	}

	void Mesh::UpdateVertices(void* _dataPointer, size_t _dataStructureSize, unsigned _vertexCount)
	{

		long * temp_pointer = (long *)_dataPointer;
		temp_pointer++;
		_dataPointer = (void*)temp_pointer;

		PE_GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		PE_GL(glBufferData(GL_ARRAY_BUFFER, _dataStructureSize * _vertexCount, _dataPointer, GL_STATIC_DRAW));

	}
}
