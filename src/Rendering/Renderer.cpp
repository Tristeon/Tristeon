#include "Renderer.h"
#include <QOpenGLShaderProgram>
#include <Scenes/Scene.h>
#include <Scenes/Layers/Layer.h>

#include "GameView.h"

namespace Tristeon
{
	bool Renderer::showGrid;
	Vector<Shader*> Renderer::prepass;
	
	void Renderer::registerPrePassShader(Shader* shader)
	{
		prepass.push_back(shader);
	}

	void Renderer::deregisterPrePassShader(Shader* shader)
	{
		prepass.remove(shader);
	}

	void Renderer::renderScene(Scene* scene)
	{
		if (scene == nullptr)
			return;

		//Send common data to all shaders through a prepass
		for (size_t i = 0; i < prepass.size() ; i++)
		{
			Shader* shader = prepass[i];
			QOpenGLShaderProgram* program = shader->getShaderProgram();

			program->bind();
			
			//Upload camera settings to the shader
			program->setUniformValue("camera.posX", scene->getCamera()->position.x);
			program->setUniformValue("camera.posY", scene->getCamera()->position.y);
			program->setUniformValue("camera.pixelsX", (int)scene->getCamera()->size.x);
			program->setUniformValue("camera.pixelsY", (int)scene->getCamera()->size.y);
			program->setUniformValue("camera.displayPixelsX", (int)GameView::width());
			program->setUniformValue("camera.displayPixelsY", (int)GameView::height());
			program->setUniformValue("camera.zoom", scene->getCamera()->zoom);
			program->setUniformValue("camera.showGrid", showGrid);
		}

		//Render each layer
		for (unsigned int i = 0; i < scene->getLayerCount(); i++)
		{
			Layer* layer = scene->getLayer(i);
			layer->render(this, scene);
		}
	}

	void Renderer::renderHUD(HUD* hud)
	{
		//TODO: HUD rendering
	}
}
