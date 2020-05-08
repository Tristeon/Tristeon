#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "Dynamic/Scene/SceneEditor.h"
#include "EditorWindow.h"
#include "Rendering/GameView.h"

namespace TristeonEditor
{
	class GameViewEditor : public Tristeon::GameView, public EditorWindow
	{
	public:
		GameViewEditor(Tristeon::Engine* engine, QWidget* parent) : GameView(engine, parent) { }
		virtual ~GameViewEditor() = default;
	protected:
		void initialize() override;
		void paintGL() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void mouseMoveEvent(QMouseEvent* event) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

		QGridLayout* layout = nullptr;
		SceneEditor* layerView = nullptr;
	};
}
#endif