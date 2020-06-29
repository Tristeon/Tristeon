#pragma once
#ifdef TRISTEON_EDITOR
#include "DockWidget.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class ObjectEditor;
	
	class LayerEditor : public QDockWidget, public DockWidget
	{
	public:
		explicit LayerEditor(QWidget* parent) : QDockWidget(parent) { }
	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

		ObjectEditor* current = nullptr;
		QWidget* contents = nullptr;
		QGridLayout* layout = nullptr;
	};
}

#endif