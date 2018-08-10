#pragma once

#include <glad/glad.h>
#include <iostream>

// This file would be included whenever we want openGL calls made.

/*@see https://www.youtube.com/watch?v=FBbPWSOQ0-w */
// We would define the OpenGL error logging macros here.
#define PE_ASSERT(x) if (!(x)) __debugbreak()

/* Check this. */
#if _DEBUG
#define PE_EXPECT(x) if (!(x)) __debugbreak()
#elif
#define PE_EXPECT(x) x
#endif

#define PE_GL(x) pe_gl_clear_errors();\
	x;\
	PE_EXPECT(pe_gl_log_error(#x, __FILE__, __LINE__))

static void pe_gl_clear_errors()
{
	while (glGetError() != GL_NO_ERROR)
	{
		// If No Errors, quit.
	}
}

static bool pe_gl_log_error(const char * _functionName, const char * _fileName, int _line)
{
	while ( GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: [" << std::hex << error << std::dec << "] at " << _functionName << " in " << _fileName << ":" << _line << std::endl;
		return false;
	}
	return true;
}

#define GL_CALL 