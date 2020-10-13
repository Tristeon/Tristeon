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
		void mousePressEvent(QMouseEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* event) override;
		void mouseMoveEvent(QMouseEvent* event) override;

	private:
		void loadProjectCache();

		void createProject();
		void openProject();

		void projectClicked(Tristeon::String path);

		void loadProject(Tristeon::String path);

		void loadSettingsWidget(QWidget* projectWidget);
		
		Tristeon::List<QWidget*> projectWidgets;
		Tristeon::List<QMetaObject::Connection> activeConnections;

		QWidget* selectedProject = nullptr;
	};
}

#endif