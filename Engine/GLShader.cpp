#include "GLShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace piolot
{
	GLShader::GLShader(const char* _vertexPath, const char* _fragmentPath)
	{

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(_vertexPath);
			fShaderFile.open(_fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure& _)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		bool success;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		success = CheckCompileErrors(vertex, "VERTEX");
		compileStatus = compileStatus && success;
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		success = CheckCompileErrors(fragment, "FRAGMENT");
		compileStatus = compileStatus && success;
		// shader Program
		shaderId = glCreateProgram();
		glAttachShader(shaderId, vertex);
		glAttachShader(shaderId, fragment);
		glLinkProgram(shaderId);
		success = CheckCompileErrors(shaderId, "PROGRAM");
		compileStatus = compileStatus && success;
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		std::string vertex_path = std::string(_vertexPath);
		shaderName = vertex_path.substr(vertex_path.find_last_of('/') + 1, vertex_path.find_last_of('.') - vertex_path.find_last_of('/') - 1);

	}

	GLShader::GLShader(const char* _combinedFilePath)
	{

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertex_code;
		std::string geometry_shader_code = "";
		std::string fragment_code;

		std::ifstream shader_file_stream;

		// ensure ifstream objects can throw exceptions:
		shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shader_file_stream = std::ifstream(_combinedFilePath);
			std::string line;

			std::stringstream ss[3];
			int stream_index = -1;

			while (getline(shader_file_stream, line))
			{
				if (line.find("#shader") != std::string::npos) {
					if (line.find("vertex") != std::string::npos) {
						stream_index = 0;
					}
					else if (line.find("fragment") != std::string::npos) {
						stream_index = 1;
					}
					else if (line.find("geometry") != std::string::npos) {
						stream_index = 2;
					}
				}
				else {
					ss[stream_index] << line << '\n';
				}
			}

			vertex_code = ss[0].str();
			fragment_code = ss[1].str();

			geometry_shader_code = ss[2].str();

		}
		catch (std::ifstream::failure& _)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertex_code.c_str();
		const char * fShaderCode = fragment_code.c_str();

		const char * g_shader_code;
		if (!geometry_shader_code.empty()) {
			g_shader_code = geometry_shader_code.c_str();
		}

		// Compile the shaders
		unsigned int vertex, fragment, geometry;

		bool success;

		// vertex shader
		PE_GL(vertex = glCreateShader(GL_VERTEX_SHADER));
		PE_GL(glShaderSource(vertex, 1, &vShaderCode, NULL));
		PE_GL(glCompileShader(vertex));
		success = CheckCompileErrors(vertex, "VERTEX");
		compileStatus = compileStatus && success;

		// fragment Shader
		PE_GL(fragment = glCreateShader(GL_FRAGMENT_SHADER));
		PE_GL(glShaderSource(fragment, 1, &fShaderCode, NULL));
		PE_GL(glCompileShader(fragment));
		success = CheckCompileErrors(fragment, "FRAGMENT");
		compileStatus = compileStatus && success;

		if (!geometry_shader_code.empty()) {
			// Geometry Shader
			PE_GL(geometry = glCreateShader(GL_GEOMETRY_SHADER));
			PE_GL(glShaderSource(geometry, 1, &g_shader_code, NULL));
			PE_GL(glCompileShader(geometry));
			success = CheckCompileErrors(geometry, "GEOMETRY");
			compileStatus = compileStatus && success;
		}


		// shader Program
		shaderId = glCreateProgram();
		glAttachShader(shaderId, vertex);
		glAttachShader(shaderId, fragment);

		if (!geometry_shader_code.empty()) {
			PE_GL(glAttachShader(shaderId, geometry));
		}

		PE_GL(glLinkProgram(shaderId));
		success = CheckCompileErrors(shaderId, "PROGRAM");
		compileStatus = compileStatus && success;
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		if (!geometry_shader_code.empty()) {
			PE_GL(glDeleteShader(geometry));
		}

	}

	bool GLShader::CheckCompileErrors(unsigned _shaderId, std::string _type)
	{
		int success;
		char infoLog[1024];
		if (_type != "PROGRAM")
		{
			glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(_shaderId, 1024, NULL, infoLog);
				std::string out = "ERROR::SHADER_COMPILATION_ERROR of type: " + _type;
				std::cout << out << std::endl;
				std::cout << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(_shaderId, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(_shaderId, 1024, NULL, infoLog);
				std::string out = "ERROR::PROGRAM_LINKING_ERROR of type: " + _type;
				std::cout << out << std::endl;
				std::cout << infoLog << std::endl;
			}
		}
		return success;
	}
}
