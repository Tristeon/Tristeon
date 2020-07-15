#ifdef TRISTEON_EDITOR
#include <glad/glad.h>

#include "GameView.h"
#include <Engine.h>
#include <Project.h>

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
		show();

		//Disables sampling because it harms 2D transparency
		QSurfaceFormat format;
		format.setRenderableType(QSurfaceFormat::OpenGL);
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setSamples(0);
		format.setSwapBehavior(Project::Graphics::tripleBuffering() ? QSurfaceFormat::TripleBuffer : QSurfaceFormat::SwapBehavior::DoubleBuffer);
		format.setSwapInterval(0);
		setFormat(format);
	}

	void GameView::initialize()
	{
		Renderer::showGrid = true;

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
			std::cout << "Error initializing glad." << std::endl;
			throw std::exception("Failed to initialize glad");
		}
		std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";
		
		QOpenGLFunctions* f = context()->functions();
		f->glClearColor(0, 0, 0, 1);

		//Enable culling
		f->glEnable(GL_CULL_FACE);
		f->glCullFace(GL_BACK);

		//Enable transparency blending
		f->glEnable(GL_BLEND);
		f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Disable multisampling because any form of aliasing messes with transparency in 2D
		f->glDisable(GL_MULTISAMPLE);
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
			layout->removeWidget(layerView);
			delete layerView;
			layerView = nullptr;
		}
	}

	void GameView::selectedLayerChanged(Tristeon::Layer* layer)
	{
		if (layerView != nullptr)
		{
			layout->removeWidget(layerView);
			delete layerView;
			layerView = nullptr;
		}

		if (layer == nullptr)
			return;

		layerView = TristeonEditor::SceneEditorRegister::createInstance(layer->serialize()["typeID"]);
		if (layerView != nullptr)
		{
			layerView->target(layer);
			layerView->setParent(this);
			layerView->resize(width(), height());
			layerView->initialize();
			layerView->setAttribute(Qt::WA_TranslucentBackground);
			layout->addWidget(layerView);
			layerView->show();
		}
	}
}
#endif