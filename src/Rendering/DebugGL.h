#pragma once
#include <glad/glad.h>

namespace Tristeon
{
	inline GLenum glCheckError_(const char* file, int line);

#ifdef TRISTEON_LOGENABLED
	/**
	 * Call after every opengl call. Will check for errors in debug mode
	 */
#define TRISTEON_DEBUG_GL() glCheckError_(__FILE__, __LINE__)
	
#else
#define TRISTEON_DEBUG_GL()
#endif
}
