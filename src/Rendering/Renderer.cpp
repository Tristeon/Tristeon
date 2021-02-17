#include "Renderer.h"

#include <Scenes/Scene.h>
#include <Scenes/Layers/Layer.h>

#include "Engine.h"
#include "Window.h"
#include "Shader.h"
#include "Gizmos.h"
#include "Grid.h"
#include "Rendering/Camera.h"
#include "Collector.h"
#include "glad/glad.h"

#include "Scenes/SceneManager.h"
#include "Scenes/Actors/Light.h"

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

			auto lights = Collector<Light>::all();
			for (size_t i = 0; i < lights.size(); i++)
			{
				auto pos = lights[i]->actor()->position;
				auto col = lights[i]->colour();
				shader->setUniformValue("lights[" + std::to_string(i) + "]" + ".position", pos.x, pos.y, -256.0f);
				shader->setUniformValue("lights[" + std::to_string(i) + "]" + ".intensity", lights[i]->intensity());
				shader->setUniformValue("lights[" + std::to_string(i) + "]" + ".color", col.r, col.g, col.b);
				shader->setUniformValue("lights[" + std::to_string(i) + "]" + ".range", lights[i]->range());
				shader->setUniformValue("lights[" + std::to_string(i) + "]" + ".type", (int)lights[i]->type());
			}
			shader->setUniformValue("lightCount", (int)lights.size());

			shader->setUniformValue("disableLighting", false);
#ifdef TRISTEON_EDITOR
			if (camera == _editorCamera.get())
				shader->setUniformValue("disableLighting", true);
#endif
		}

		//Render each layer
		for (unsigned int i = 0; i < SceneManager::current()->layerCount(); i++)
		{
			Layer* layer = SceneManager::current()->layerAt(i);
			layer->render(Framebuffer{ camera->_fbo, { 0, 0, resolution.x, resolution.y } });
		}

#ifdef TRISTEON_EDITOR
		if (camera == _editorCamera.get())
		{
			Grid::render();
			Gizmos::render();
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
