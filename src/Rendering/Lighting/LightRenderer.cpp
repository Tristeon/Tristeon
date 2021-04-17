#include "LightRenderer.h"
#include <glad/glad.h>
#include <Collector.h>

#include <Rendering/Camera.h>
#include <Rendering/RenderMask.h>

#include <Rendering/Lighting/CompositeLight.h>
#include <Rendering/Lighting/AmbientLight.h>
#include <Rendering/Lighting/PointLight.h>
#include <Rendering/Lighting/SpotLight.h>

namespace Tristeon
{
	void LightRenderer::renderComposite(Camera* pCamera)
	{
		const auto resolution = pCamera->resolution();
		
		glBlendFunc(GL_ONE, GL_ONE);
		for (auto i = 0; i < 8; i++)
		{
			auto mask = (RenderMask)(1 << i);
			glBindFramebuffer(GL_FRAMEBUFFER, pCamera->_compositeLightFBOs[i]);
			glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto* shape : Collector<CompositeLight>::all())
			{
				if (((int)shape->_renderMask & (int)mask) == (int)mask)
					shape->render();
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LightRenderer::bindCompositeTextures(Camera* pCamera)
	{
		for (auto i = 0; i < (int)pCamera->_compositeLightTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE10 + i);
			glBindTexture(GL_TEXTURE_2D, pCamera->_compositeLightTextures[i]);
		}
	}

	void LightRenderer::passLights()
	{
		//Pass lighting data
		for (auto* shader : Collector<Shader>::all())
		{
			if (shader->empty())
				continue;

			shader->bind();

			auto pointLights = Collector<PointLight>::all();
			uint64_t p = 0, s = 0;
			while (p + s < pointLights.size())
			{
				auto* light = pointLights[p + s];
				auto* spot = dynamic_cast<SpotLight*>(light);

				if (spot)
				{
					light->prepareRender(shader, s);
					s++;
				}
				else
				{
					light->prepareRender(shader, p);
					p++;
				}
			}
			shader->setUniformValue("spotLightCount", (int)s);
			shader->setUniformValue("pointLightCount", (int)p);

			Colour ambientLight{ 0, 0, 0, 0 };
			for (auto* ambient : Collector<AmbientLight>::all())
			{
				ambientLight = {
					ambientLight.r + ambient->colour().r * ambient->intensity(),
					ambientLight.g + ambient->colour().g * ambient->intensity(),
					ambientLight.b + ambient->colour().b * ambient->intensity(),
					0,
				};
			}
			shader->setUniformValue("ambientLight", ambientLight.r, ambientLight.g, ambientLight.b);

			for (auto i = 0; i < 8; i++)
				shader->setUniformValue("compositeLight[" + std::to_string(i) + "]", 10 + i);
		}
	}
}