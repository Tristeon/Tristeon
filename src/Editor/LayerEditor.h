#pragma once
#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"
#include <Editor/CustomEditors/CustomEditor.h>

#include <QtWidgets>

namespace TristeonEditor
{
	class LayerEditor : public QDockWidget, public EditorWindow
	{
	public:
		explicit LayerEditor(QWidget* parent) : QDockWidget(parent) { }

	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

		CustomEditor* current = nullptr;
		QWidget* contents = nullptr;
		QGridLayout* layout = nullptr;
	};
}

#endif