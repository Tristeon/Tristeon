#pragma once
#include "DockWidget.h"
#include <QtWidgets>

namespace TristeonEditor
{
	class AbstractEditor;
	
	class LayerEditor : public QDockWidget, public DockWidget
	{
	public:
		explicit LayerEditor(QWidget* parent) : QDockWidget(parent) { }
	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

		std::unique_ptr<AbstractEditor> current = nullptr;
		QWidget* contents = nullptr;
		QGridLayout* layout = nullptr;
	};
}