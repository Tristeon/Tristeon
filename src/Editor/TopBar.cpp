#ifdef TRISTEON_EDITOR
#include "TopBar.h"

namespace TristeonEditor
{
	TopBar::TopBar(QWidget* parent) : QDockWidget(parent)
	{
		auto* title = new QWidget;
		setTitleBarWidget(title);
		titleBarWidget()->hide();
	}

	void TopBar::initialize()
	{
		//Empty
	}

	void TopBar::loadScene(Tristeon::Scene* scene)
	{
		//Empty
	}
}
#endif