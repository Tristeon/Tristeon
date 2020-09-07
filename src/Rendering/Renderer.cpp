#include "Renderer.h"

#include <Scenes/Scene.h>
#include <Scenes/Layers/Layer.h>


#include "Engine.h"
#include "Window.h"
#include "Shader.h"
#include "Gizmos.h"
#include "Grid.h"
#include "Actors/Camera.h"
#include "Collectors/Collector.h"
#include "glad/glad.h"

#include "Scenes/SceneManager.h"

namespace Tristeon
{
	bool Renderer::showGrid;

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

	void Renderer::render(unsigned int const& framebuffer)
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
			camera->bindFramebuffer();
			
			//Send common data to all shaders through a prepass
			for (auto shader : Collector<Shader>::all())
			{
				if (shader->isEmpty())
					continue;
				
				shader->bind();
				shader->setUniformValue("camera.position", (float)camera->position.x, (float)camera->position.y);
				shader->setUniformValue("camera.zoom", camera->zoom);

				Vector2Int resolution = (Vector2Int)((Vector2)Window::gameSize() * camera->screenSize);
				if (!camera->renderToScreen)
					resolution = camera->overrideResolution;
				glViewport(0, 0, resolution.x, resolution.y);
				shader->setUniformValue("camera.displayPixels", (unsigned int)resolution.x, (unsigned int)resolution.y);
			}

			//Render each layer
			for (unsigned int i = 0; i < scene->getLayerCount(); i++)
			{
				Layer* layer = scene->getLayer(i);
				layer->render(this, scene);
			}
			if (showGrid)
				Grid::render();
			Gizmos::render();
		}

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