#include "PostProcessingEffect.h"
#include <glad/glad.h>

namespace Tristeon
{
	void PostProcessingEffect::render()
	{
		auto* s = shader();
		s->bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}