#pragma once
#ifdef TRISTEON_EDITOR
#include <QMenuBar>
#include <TypeDefinitions.h>

namespace TristeonEditor
{
	class MenuBar : public QMenuBar
	{
	public:
		MenuBar(QWidget* parent = nullptr) : QMenuBar(parent) {}
		void initialize();
	private:
		void createScene();
		void createTileset();
		void createAnimationClip();

		void openScene();

		void saveScene();

		void saveCurrentSceneDialog(std::string nextSceneName);

		static QAction* findAction(QMenu* menu, Tristeon::String name);
	};
}
#endif