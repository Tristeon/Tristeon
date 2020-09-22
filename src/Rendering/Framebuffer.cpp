#include "Framebuffer.h"
#include <glad/glad.h>

namespace Tristeon
{
	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	}
}