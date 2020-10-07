#pragma once
#ifdef TRISTEON_EDITOR
#include <Engine.h>

#include <Editor/DockWidget.h>
#include <Utils/Singleton.h>

#include <QOpenGLWidget>
#include <qgridlayout.h>

#include "EditorCameraController.h"
#include "Dynamic/Scene/SceneEditor.h"

namespace TristeonEditor
{
	/**
	 * GameView is a OpenGL Widget, created by Qt to which the scene/game is rendered. 
	 */
	class GameView : public QOpenGLWidget, public Tristeon::Singleton<GameView>, public DockWidget
	{
		friend Tristeon::Engine;
	public:
		explicit GameView(QWidget* parent);
		virtual ~GameView() = default;
		
	protected:
		void initialize() override;
		
		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

	private:
		QGridLayout* layout = nullptr;
		TristeonEditor::SceneEditor* layerView = nullptr;
		Tristeon::Unique<TristeonEditor::EditorCameraController> cameraController;
	};
}
#endif