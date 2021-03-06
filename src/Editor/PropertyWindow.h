#pragma once
#include <QtWidgets>
#include <Editor/DockWidget.h>
#include <Standard/Unique.h>
#include <Editor/Dynamic/AbstractEditor.h>

namespace TristeonEditor
{
	class PropertyWindow : public QDockWidget, public DockWidget
	{
	public:
		explicit PropertyWindow(QWidget* parent) : QDockWidget(parent) { }

	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedActorChanged(Tristeon::Actor* actor) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;
		void selectedFilePathChanged(FileItem* item) override;

		Tristeon::Unique<AbstractEditor> current = nullptr;
		QWidget* contents = nullptr;
		QGridLayout* layout = nullptr;
	};
}