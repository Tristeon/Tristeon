#pragma once
#ifdef TRISTEON_EDITOR

#include <QtWidgets>

#include <Editor/DockWidget.h>
#include <Scenes/Layers/Layer.h>

namespace TristeonEditor
{
	class LayerListEditor : public QDockWidget, public DockWidget
	{
	public:
		explicit LayerListEditor(QWidget* parent = nullptr);

	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;

		void addButtonPressed();
		void removeButtonPressed();
		void rowsMoved(const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row);
		void selectionChanged(QListWidgetItem* current, QListWidgetItem* previous);
		void itemRenamed(QListWidgetItem* item);
		
		QListWidget* list = nullptr;
		std::map<QListWidgetItem*, Tristeon::Layer*> layers;
	};
}
#endif