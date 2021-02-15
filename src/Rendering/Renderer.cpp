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

		for (int i = 0; i < 32; i++)
		{
			getDeferredCameraShader()->setUniformValue("lights[" + std::to_string(i) + "].position", Random::generateFloat(0, 1000), Random::generateFloat(0, 1000), 0.0f);
			getDeferredCameraShader()->setUniformValue("lights[" + std::to_string(i) + "].intensity", 1.0f);
			getDeferredCameraShader()->setUniformValue("lights[" + std::to_string(i) + "].color", Random::generateFloat01(), Random::generateFloat01(), Random::generateFloat01());
			getDeferredCameraShader()->setUniformValue("lights[" + std::to_string(i) + "].intensity", 1024.0f);
			getDeferredCameraShader()->setUniformValue("lights[" + std::to_string(i) + "].type", 0);
		}
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
			const auto offlineBuffer = camera->_offlineFBO;
			auto resolution = camera->resolution();
			glBindFramebuffer(GL_FRAMEBUFFER, offlineBuffer);
			glViewport(0, 0, resolution.x, resolution.y);
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
			for (unsigned int i = 0; i < scene->layerCount(); i++)
			{
				Layer* layer = scene->layerAt(i);
				layer->render(Framebuffer { offlineBuffer, { 0, 0, resolution.x, resolution.y }});
			}

#ifdef TRISTEON_EDITOR
			//Render grid to editor camera
			if (camera == _editorCamera.get())
				Grid::render();
#endif

			//Render gizmos
			Gizmos::render();
		}

		glUseProgram(GL_NONE);

		for (auto* camera : cameras)
		{
			//Deferred output pass
			auto outputBuffer = camera->framebuffer();
			outputBuffer.bind();
			glClear(GL_COLOR_BUFFER_BIT);

			getDeferredCameraShader()->bind();
			
			getDeferredCameraShader()->setUniformValue("albedo", 0);
			getDeferredCameraShader()->setUniformValue("normals", 1);
			getDeferredCameraShader()->setUniformValue("positions", 2);

			auto mouse = Window::screenToWorld(Mouse::position(), camera);
			getDeferredCameraShader()->setUniformValue("lights[0].position", mouse.x, mouse.y, 0.0f);

			for (int i = 0; i < camera->_offlineFBOTextures.size(); i++) 
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, camera->_offlineFBOTextures[i]);
			}

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		//Prepare renderer for rendering to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, Window::gameWidth(), Window::gameHeight());
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

	Shader* Renderer::getDeferredCameraShader()
	{
		static Shader deferredCamera("Internal/Shaders/FullscreenTriangle.vert", "Internal/Shaders/DeferredCamera.frag");

		return &deferredCamera;
	}
}
