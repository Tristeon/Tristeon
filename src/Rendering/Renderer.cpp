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
#include "Input/Mouse.h"

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
			renderDeferred(camera);
		}

		renderOnscreen(framebuffer, cameras);
	}

	Shader* Renderer::getDeferredCameraShader()
	{
		static Shader deferredCamera("Internal/Shaders/FullscreenTriangle.vert", "Internal/Shaders/DeferredCamera.frag");
		return &deferredCamera;
	}

	void Renderer::renderOffline(Camera* camera) const
	{
		const auto resolution = camera->resolution();
		glBindFramebuffer(GL_FRAMEBUFFER, camera->_offlineFBO);
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
		}

		//Render each layer
		for (unsigned int i = 0; i < SceneManager::current()->layerCount(); i++)
		{
			Layer* layer = SceneManager::current()->layerAt(i);
			layer->render(Framebuffer{ camera->_offlineFBO, { 0, 0, resolution.x, resolution.y } });
		}

#ifdef TRISTEON_EDITOR
		//Render grid to editor camera
		if (camera == _editorCamera.get())
			Grid::render();
#endif

		//Render gizmos
		Gizmos::render();
	}

	void Renderer::renderDeferred(Camera* camera)
	{
		const auto resolution = camera->resolution();
		
		glBindFramebuffer(GL_FRAMEBUFFER, camera->_fbo);
		glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
		glClear(GL_COLOR_BUFFER_BIT);

		auto* shader = getDeferredCameraShader();
		shader->bind();

		shader->setUniformValue("albedo", 0);
		shader->setUniformValue("normals", 1);
		shader->setUniformValue("positions", 2);

		shader->setUniformValue("lights[0].position", 500.0f, 800.0f, -256.0f);

		for (size_t i = 0; i < camera->_offlineFBOTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, camera->_offlineFBOTextures[i]);
		}

		glDrawArrays(GL_TRIANGLES, 0, 3);
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
