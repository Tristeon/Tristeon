#include <glad/glad.h>

#include "GameView.h"
#include <Engine.h>
#include <Settings.h>

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>

#include "EditorCameraController.h"
#include "Dynamic/Scene/SceneEditorRegister.h"

#include <QApplication>
#include <QOpenGLFunctions>
#include <qgridlayout.h>

using namespace Tristeon;

namespace TristeonEditor
{
	GameView::GameView(QWidget* parent) : QOpenGLWidget(parent)
	{
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setSamples(0);
        format.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
        format.setSwapInterval(0);
        setFormat(format);

        show();
	}

	void GameView::initialize()
	{
		layout = new QGridLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);

		cameraController = std::make_unique<TristeonEditor::EditorCameraController>();
	}

	void GameView::initializeGL()
	{
		//Load OGL
		if (!gladLoadGL())
		{
			TRISTEON_ERROR("Error initializing glad");
			return;
		}
		TRISTEON_LOG("Detected OpenGL: " + std::to_string(GLVersion.major) + "." + std::to_string(GLVersion.minor));
		if (GLVersion.major < 3)
        {
		    TRISTEON_ERROR("Incorrect OpenGL version!");
			return;
        }

		glad_glClearColor(0, 0, 0, 1);

		//Enable culling
		glad_glEnable(GL_CULL_FACE);
		glad_glCullFace(GL_BACK);

		//Enable transparency blending
		glad_glEnable(GL_BLEND);
		glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Disable multisampling because any form of aliasing messes with transparency in 2D
		glad_glDisable(GL_MULTISAMPLE);

		glad_glDisable(GL_DEPTH_TEST);
		glad_glDisable(GL_STENCIL_TEST);
	}

	void GameView::resizeGL(int w, int h)
	{
		paintGL();
	}

	void GameView::paintGL()
	{
		//Update editor information
		if (Tristeon::Engine::playMode() == false)
		{
			if (cameraController != nullptr)
				cameraController->update();
		}
		if (layerView != nullptr)
			layerView->updateView();

		//Draw
		QOpenGLFunctions* f = context()->functions();
		f->glClear(GL_COLOR_BUFFER_BIT);

		Scene* scene = SceneManager::current();
		if (scene != nullptr)
		{
			Renderer::instance()->render(context()->defaultFramebufferObject());
		}
	}

	void GameView::sceneLoaded(Tristeon::Scene* scene)
	{
		if (layerView != nullptr)
		{
			layout->removeWidget(layerView.get());
			layerView.reset();
		}
	}

	void GameView::selectedLayerChanged(Tristeon::Layer* layer)
	{
		if (layerView != nullptr)
		{
			layout->removeWidget(layerView.get());
			layerView.reset();
		}

		if (layer == nullptr)
			return;

		layerView = TristeonEditor::SceneEditorRegister::createInstance(layer->serialize()["typeID"], layer);
		if (layerView != nullptr)
		{
			layout->addWidget(layerView.get());
			layerView->resize(width(), height());
			layerView->setAttribute(Qt::WA_TranslucentBackground);
		}
	}
}