﻿#pragma once

#include "PE_GL.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace piolot
{
	/**
	* \brief The Class that is used to load shaders in OpenGL.
	*/
	class GLShader
	{
	public:

		/**
		* \brief The Shader ID that you get once you load the Shader.
		*/
		unsigned int shaderId;

		bool compileStatus = true;

		/**
		* \brief Loads the shader at the specified path.
		* \param _vertexPath The VertexShader Path
		* \param _fragmentPath The Fragment Shader Path.
		*
		*	-	Reads the files specified by Shaderpaths.
		*	-	Creates the Shaders and Compiles them.
		*	-	Creates the Program and links the shaders.
		*	-	Deletes the Shaders once the Program is successfully created.
		*	-	Outputs any errors to the debug file.
		*/
		GLShader(const char * _vertexPath, const char * _fragmentPath);

		/**
		* \brief Default Destructor
		*/
		~GLShader() = default;

		/**
		* \brief Use the Program.
		*/
		void use()
		{
			PE_GL(glUseProgram(shaderId));
		}


		/**
		* \brief Set a Uniform Value
		* \param _name The Uniform Name
		* \param _value The Uniform Value.
		*/
		void setBool(const std::string &_name, bool _value) const
		{
			PE_GL(glUniform1i(glGetUniformLocation(shaderId, _name.c_str()), (int)_value));
		}

		/**
		* \brief Set a Uniform Value
		* \param _name The Uniform Name
		* \param _value The Uniform Value.
		*/
		void setInt(const std::string &_name, int _value) const
		{
			PE_GL(glUniform1i(glGetUniformLocation(shaderId, _name.c_str()), _value));
		}

		/**
		* \brief Set a Uniform Value
		* \param _name The Uniform Name
		* \param _value The Uniform Value.
		*/
		void setFloat(const std::string &_name, float _value) const
		{
			PE_GL(glUniform1f(glGetUniformLocation(shaderId, _name.c_str()), _value));
		}

		/**
		* \brief Set a Uniform Value
		* \param _name The Uniform Name
		* \param _value The Uniform Value.
		*/
		void setVec3(const std::string &_name, const glm::vec3 &_value) const
		{
			PE_GL(glUniform3fv(glGetUniformLocation(shaderId, _name.c_str()), 1, &_value[0]));
		}

		/**
		* \brief Set a Uniform Value
		* \param _name The Uniform Name
		* \param _x	Value.x
		* \param _y Value.y
		* \param _z Value.z
		*/
		void setVec3(const std::string &_name, float _x, float _y, float _z) const
		{
			PE_GL(glUniform3f(glGetUniformLocation(shaderId, _name.c_str()), _x, _y, _z));
		}

		/**
		* \brief Set a Uniform Value.
		* \param _name The Uniform Name
		* \param _mat The Uniform Value.
		*/
		void setMat4(const std::string &_name, const glm::mat4 &_mat) const
		{
			PE_GL(const auto location = glGetUniformLocation(shaderId, _name.c_str()));
			PE_GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_mat)));
		}

		/**
		* \brief Check for any Compilation Errors and report.
		* \param _shaderId The Shader/Program Id that we are checking the errors for.
		* \param _type The Type, Can be either PROGRAM or SHADER.
		*/
		bool CheckCompileErrors(unsigned int _shaderId, std::string _type);

	};
}