#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include <Editor/EditorWindow.h>
#include <Editor/CustomEditors/CustomEditor.h>
namespace TristeonEditor
{
	class PropertyWindow : public QDockWidget, public EditorWindow
	{
	public:
		explicit PropertyWindow(QWidget* parent) : QDockWidget(parent) { }

	protected:
		void initialize() override;
		void sceneLoaded(Tristeon::Scene* scene) override;
		void selectedActorChanged(Tristeon::Actor* actor) override;
		void selectedLayerChanged(Tristeon::Layer* layer) override;

		CustomEditor* current = nullptr;
		QWidget* contents = nullptr;
		QGridLayout* layout = nullptr;
	};
}
#endif