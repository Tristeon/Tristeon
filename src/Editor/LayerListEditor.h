#pragma once
#ifdef TRISTEON_EDITOR

#include <QtWidgets>

#include <Editor/EditorWindow.h>
#include <Scenes/Layers/Layer.h>

namespace TristeonEditor
{
	class LayerListEditor : public QDockWidget, public EditorWindow
	{
	public:
		explicit LayerListEditor(QWidget* parent = nullptr);

	protected:
		void initialize() override;
		void loadScene(Tristeon::Scene* scene) override;

		void addButtonPressed();
		void removeButtonPressed();

		QListWidget* list = nullptr;
		std::map<QListWidgetItem*, Tristeon::Layer*> layers;
	};
}
#endif