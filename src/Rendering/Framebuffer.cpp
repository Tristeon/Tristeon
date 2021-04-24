#include "Framebuffer.h"
#include <glad/glad.h>

#include "DebugGL.h"

namespace Tristeon
{
	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		TRISTEON_DEBUG_GL();
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
		TRISTEON_DEBUG_GL();
	}
}
