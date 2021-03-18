#include "Renderer.h"

#include <Rendering/Lighting/AmbientLight.h>
#include <Rendering/Lighting/PointLight.h>
#include <Rendering/Lighting/SpotLight.h>

#include <Scenes/Scene.h>
#include <Scenes/Layers/ActorLayer.h>
#include <Scenes/Layers/Layer.h>

#include <Collector.h>
#include <Engine.h>
#include <Window.h>
#include <glad/glad.h>
#include <Rendering/Camera.h>
#include <Rendering/Gizmos.h>
#include <Rendering/Grid.h>
#include <Rendering/Shader.h>
#include <Scenes/SceneManager.h>

#include "Lighting/CompositeLight.h"

namespace Tristeon
{
	Renderer::Renderer()
	{
#ifdef TRISTEON_EDITOR
		_editorCamera = std::make_unique<Camera>(false);
		_editorCamera->renderToScreen = true;
		_editorCamera->screenCoordinates = { -1, -1 };
		_editorCamera->screenSize = { 1, 1 };
		_editorCamera->zoom = 0.25f;
#endif

		glGenVertexArrays(1, &_dummyVAO);
		glBindVertexArray(_dummyVAO);
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &_dummyVAO);
#ifdef TRISTEON_EDITOR
		_editorCamera->destroy();
#endif
	}

	void Renderer::render(const unsigned int& framebuffer)
	{
		Scene* scene = SceneManager::current();
		if (scene == nullptr)
			return;

		auto cameras = Collector<Camera>::all();
		cameras.sort([](Camera* a, Camera* b) { return a->renderToScreen < b->renderToScreen; });

#ifdef TRISTEON_EDITOR
		if (!Engine::playMode())
			cameras.add(editorCamera());
#endif

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
		}

		for (auto* camera : cameras)
		{
			camera->updateFramebuffers();
			renderOffline(camera);
		}

		renderOnscreen(framebuffer, cameras);
	}

	void Renderer::renderOffline(Camera* camera) const
	{
		const auto resolution = camera->resolution();
		glBindFramebuffer(GL_FRAMEBUFFER, camera->_fbo);
		glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
		glClear(GL_COLOR_BUFFER_BIT);

		//Send common data to all shaders through a prepass
		for (auto* shader : Collector<Shader>::all())
		{
			if (shader->empty())
				continue;

			shader->bind();
			shader->setUniformValue("camera.position", camera->position.x, camera->position.y);
			shader->setUniformValue("camera.zoom", camera->zoom);
			shader->setUniformValue("camera.displayPixels", resolution.x, resolution.y);

#ifdef TRISTEON_EDITOR
			if (camera == _editorCamera.get())
				shader->setUniformValue("disableLighting", _editorLightingDisabled);
			else
#endif
				shader->setUniformValue("disableLighting", false);
			for (auto i = 0; i < 8; i++)
				shader->setUniformValue("compositeLight[" + std::to_string(i) + "]", 10 + i);
		}

		//Render composite light
		glBlendFunc(GL_ONE, GL_ONE);
		for (auto i = 0; i < 8; i++)
		{
			auto mask = (RenderMask)(1 << i);
			glBindFramebuffer(GL_FRAMEBUFFER, camera->_compositeLightFBOs[i]);
			glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto* shape : Collector<CompositeLight>::all())
			{
				if (((int)shape->_mask & (int)mask) == (int)mask)
					shape->render();
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Bind all composite light textures
		for (auto i = 0; i < (int)camera->_compositeLightTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE10 + i);
			glBindTexture(GL_TEXTURE_2D, camera->_compositeLightTextures[i]);
		}
		
		//Render each layer
		const auto framebuffer = Framebuffer{ camera->_fbo, { 0, 0, resolution.x, resolution.y } };
		glBindFramebuffer(GL_FRAMEBUFFER, camera->_fbo);
		for (unsigned int i = 0; i < SceneManager::current()->layerCount(); i++)
		{
			SceneManager::current()->layerAt(i)->render(framebuffer);
		}

#ifdef TRISTEON_EDITOR
		if (camera == _editorCamera.get())
		{
			if (_editorGridEnabled) Grid::render();
			if (_editorGizmosEnabled) Gizmos::render();
		}
#endif
	}

	void Renderer::renderOnscreen(const unsigned int& framebuffer, const List<Camera*>& cameras) const
	{
		//Prepare renderer for rendering to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, (GLsizei)Window::gameWidth(), (GLsizei)Window::gameHeight());
		glClear(GL_COLOR_BUFFER_BIT);

		if (Engine::playMode())
		{
			for (auto* camera : cameras)
			{
#ifdef TRISTEON_EDITOR
				if (camera == _editorCamera.get())
					continue;
#endif
				if (camera->renderToScreen)
					camera->drawToScreen();
			}
		}
#ifdef TRISTEON_EDITOR
		else
		{
			_editorCamera->drawToScreen();
		}
#endif
		Gizmos::clear();
	}
}
