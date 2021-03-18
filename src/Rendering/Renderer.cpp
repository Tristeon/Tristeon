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

	void Renderer::render(const unsigned int& framebuffer) const
	{
		auto* scene = SceneManager::current();
		if (scene == nullptr)
			return;

		auto cameras = Collector<Camera>::all();
		cameras.sort([](Camera* a, Camera* b) { return a->renderToScreen < b->renderToScreen; });

#ifdef TRISTEON_EDITOR
		if (!Engine::playMode())
			cameras.add(editorCamera());
#endif

		LightRenderer::passLights();

		for (auto* camera : cameras)
		{
			camera->updateFramebuffers();
			renderOffline(camera);
		}

		renderOnscreen(framebuffer, cameras);
	}

	void Renderer::passCameraData(Camera* pCamera) const
	{
		const auto resolution = pCamera->resolution();
		for (auto* shader : Collector<Shader>::all())
		{
			if (shader->empty())
				continue;

			shader->bind();
			shader->setUniformValue("camera.position", pCamera->position.x, pCamera->position.y);
			shader->setUniformValue("camera.zoom", pCamera->zoom);
			shader->setUniformValue("camera.displayPixels", resolution.x, resolution.y);

#ifdef TRISTEON_EDITOR
			if (pCamera == _editorCamera.get())
				shader->setUniformValue("disableLighting", _editorLightingDisabled);
			else
#endif
				shader->setUniformValue("disableLighting", false);
		}
	}

	void Renderer::renderOffline(Camera* pCamera) const
	{
		//Pass render data & render lights
		passCameraData(pCamera);
		LightRenderer::renderComposite(pCamera);
		LightRenderer::bindCompositeTextures(pCamera);

		//Prepare camera framebuffer
		const auto resolution = pCamera->resolution();
		glBindFramebuffer(GL_FRAMEBUFFER, pCamera->_fbo);
		glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render each layer
		const auto framebuffer = Framebuffer{ pCamera->_fbo, { 0, 0, resolution.x, resolution.y } };
		glBindFramebuffer(GL_FRAMEBUFFER, pCamera->_fbo);
		for (unsigned int i = 0; i < SceneManager::current()->layerCount(); i++)
		{
			SceneManager::current()->layerAt(i)->render(framebuffer);
		}

		//Render grid/gizmos if this is the editor camera
#ifdef TRISTEON_EDITOR
		if (pCamera == _editorCamera.get())
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