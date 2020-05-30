#pragma once
#ifdef TRISTEON_EDITOR
#include <QMainWindow>

namespace TristeonEditor
{
	class ProjectWindow : public QMainWindow
	{
	public:
		explicit ProjectWindow(QWidget* parent = nullptr);

	protected:
		void mouseDoubleClickEvent(QMouseEvent* event) override;
		void mouseMoveEvent(QMouseEvent* event) override;

	private:
		void loadProjectCache();

		void createProject();
		void openProject();

		void projectClicked(Tristeon::String path);

		void loadProject(Tristeon::String path);
		
		Tristeon::Vector<QWidget*> projectWidgets;
	};
}

#endif