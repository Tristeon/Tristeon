#include "Renderer.h"

#include <Scenes/Scene.h>
#include <Scenes/Layers/Layer.h>

#include "Window.h"
#include "Shader.h"
#include "Gizmos.h"
#include "Grid.h"

#include "Scenes/SceneManager.h"

namespace Tristeon
{
	bool Renderer::showGrid;
	Vector<Shader*> Renderer::prepass;
	
	void Renderer::registerPrePassShader(Shader* shader)
	{
		prepass.add(shader);
	}

	void Renderer::deregisterPrePassShader(Shader* shader)
	{
		prepass.remove(shader);
	}

	void Renderer::render(unsigned int const& framebuffer) 
	{
		Scene* scene = SceneManager::current();
		if (scene == nullptr)
			return;

		//Send common data to all shaders through a prepass
		for (size_t i = 0; i < prepass.size() ; i++)
		{
			Shader* shader = prepass[i];
			shader->bind();
			shader->setUniformValue("camera.posX", scene->getCamera()->position.x);
			shader->setUniformValue("camera.posY", scene->getCamera()->position.y);
			shader->setUniformValue("camera.pixelsX", (unsigned int)scene->getCamera()->size.x);
			shader->setUniformValue("camera.pixelsY", (unsigned int)scene->getCamera()->size.y);
			shader->setUniformValue("camera.displayPixelsX", Window::gameWidth());
			shader->setUniformValue("camera.displayPixelsY", Window::gameHeight());
			shader->setUniformValue("camera.zoom", scene->getCamera()->zoom);
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
}
