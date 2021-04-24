#pragma once
#include <glad/glad.h>

namespace Tristeon
{
	struct DebugGL
	{
		static GLenum checkError(const char* file, int line);
	};
}

#ifdef TRISTEON_LOGENABLED
/**
 * Call after every opengl call. Will check for errors in debug mode
 */
#define TRISTEON_DEBUG_GL() Tristeon::DebugGL::checkError(__FILE__, __LINE__)

#else
#define TRISTEON_DEBUG_GL()
#endif