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
	}

	Renderer::~Renderer()
	{
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
			auto buffer = camera->framebuffer();
			buffer.bind();
			glClear(GL_COLOR_BUFFER_BIT);
			
			//Send common data to all shaders through a prepass
			for (auto shader : Collector<Shader>::all())
			{
				if (shader->empty())
					continue;
				
				shader->bind();
				shader->setUniformValue("camera.position", camera->position.x, camera->position.y);
				shader->setUniformValue("camera.zoom", camera->zoom);
				shader->setUniformValue("camera.displayPixels", buffer.viewport.width, buffer.viewport.height);
			}

			//Render each layer
			for (unsigned int i = 0; i < scene->layerCount(); i++)
			{
				Layer* layer = scene->layerAt(i);
				layer->render(buffer);
			}

#ifdef TRISTEON_EDITOR
			//Render grid to editor camera
			if (camera == _editorCamera.get())
				Grid::render();
#endif

			//Render gizmos
			Gizmos::render();
		}

		//Prepare renderer for rendering to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, Window::gameWidth(), Window::gameHeight());

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