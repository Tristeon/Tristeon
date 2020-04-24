#pragma once
#ifdef TRISTEON_EDITOR
#include "EditorWindow.h"
#include <QDockWidget>

namespace TristeonEditor
{
	class TopBar : public QDockWidget, public EditorWindow
	{
	public:
		explicit TopBar(QWidget* parent = nullptr);
	protected:
		void initialize() override;
		void loadScene(Tristeon::Scene* scene) override;
	};
}
#endif