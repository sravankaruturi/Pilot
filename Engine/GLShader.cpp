#include "GLShader.h"


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
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	bool success;
	char infoLog[512];
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
			std::string out = "ERROR::SHADER_COMPILATION_ERROR of type: ";
			std::string out = "ERROR::SHADER_COMPILATION_ERROR of type: " + _type;
		}
	}
	else
	{
		glGetProgramiv(_shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(_shaderId, 1024, NULL, infoLog);
		}
	}
	return success;
}
