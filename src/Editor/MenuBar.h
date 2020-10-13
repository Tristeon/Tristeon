#pragma once
#ifdef TRISTEON_EDITOR
#include <QMenuBar>
#include <Standard/String.h>

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

		void saveCurrentSceneDialog(Tristeon::String nextSceneName);

		static QAction* findAction(QMenu* menu, Tristeon::String name);
	};
}
#endif