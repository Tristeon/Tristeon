#ifdef TRISTEON_EDITOR
#include "Scenes/Scene.h"
#include "Project.h"
#include "ProjectWindow.h"
#include <quiloader.h>
#include <QtUiTools>
#include <Serialization/JsonSerializer.h>

namespace TristeonEditor
{
	ProjectWindow::ProjectWindow(QWidget* parent): QMainWindow(parent)
	{
		setMouseTracking(true);
		resize(960, 540);
		show();

		QUiLoader loader;
		QFile projectFile("ProjectEditor.ui");
		projectFile.open(QFile::ReadOnly);

		QWidget* widget = loader.load(&projectFile);
		setCentralWidget(widget);

		loadProjectCache();

		auto* new_project = widget->findChild<QToolButton*>("new_project");
		connect(new_project, &QToolButton::clicked, this, &ProjectWindow::createProject);

		auto* open_project = widget->findChild<QToolButton*>("open_project");
		connect(open_project, &QToolButton::clicked, this, &ProjectWindow::openProject);
	}

	void ProjectWindow::mouseDoubleClickEvent(QMouseEvent* event)
	{
		for (auto const& projectWidget : projectWidgets)
		{
			if (!projectWidget->isVisible())
				continue;

			if (projectWidget->underMouse() && event->button() == Qt::LeftButton)
				projectClicked(projectWidget->objectName().toStdString());
		}
	}

	void ProjectWindow::mouseMoveEvent(QMouseEvent* event)
	{
		for (auto const& projectWidget : projectWidgets)
		{
			if (projectWidget->underMouse())
			{
				projectWidget->setStyleSheet("background-color: rgb(60, 60, 60)");
			}
			else
			{
				projectWidget->setStyleSheet("");
			}
		}
	}

	void ProjectWindow::loadProjectCache()
	{
		auto* project_contents = findChild<QWidget*>("project_contents");

		json cache = Tristeon::JsonSerializer::load("Local/Projects.json");

		if (cache.is_null())
		{
			cache = json();
			cache["projects"] = json::array_t();
			Tristeon::JsonSerializer::save("Local/Projects.json", cache);
			return;
		}

		json::array_t projects = cache.value("projects", json::array_t());
		json::array_t valid_projects;

		for (auto& project : projects)
		{
			std::string path = project.get<Tristeon::String>();

			if (!QFile::exists(QString::fromStdString(path)) || QFileInfo(QString::fromStdString(path)).suffix() != "tristeon")
				continue;

			valid_projects.push_back(path);

			QDir dir = QFileInfo(QString::fromStdString(path)).dir();

			QUiLoader loader;
			QFile widgetFile("ProjectWidget.ui");
			widgetFile.open(QFile::ReadOnly);

			QWidget* projectWidget = loader.load(&widgetFile);
			projectWidget->setObjectName(QString::fromStdString(path));
			project_contents->layout()->addWidget(projectWidget);

			projectWidgets.push_back(projectWidget);

			auto* title = projectWidget->findChild<QLabel*>("title");
			title->setText(dir.dirName());

			auto* pathLabel = projectWidget->findChild<QLabel*>("path");
			pathLabel->setText(QString::fromStdString(path));
		}

		json save;
		save["projects"] = valid_projects;
		Tristeon::JsonSerializer::save("Local/Projects.json", save);

		project_contents->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	}

	void ProjectWindow::createProject()
	{
		QString const path = QFileDialog::getExistingDirectory(this, QWidget::tr("Select Project Folder"), "C:/");
		if (path.isEmpty())
			return;
		
		QDir const dir(path);
		if (!dir.isEmpty())
		{
			QMessageBox::warning(this, QWidget::tr("Warning"), QWidget::tr("The project folder must be empty!"));
			return;
		}

		json const project = {
			{
				"graphics",
				{
					{"tileWidth", 64},
					{"tileHeight", 64},
					{"vsync", false},
					{"tripleBuffering", true},
					{"fullscreen", true}
				}
			},
			{
				"physics",
				{
					{"fixedDeltaTime", 20},
					{"pixelsPerMeter", 64},
				}
			},
			{"firstScene", "Scene"}
		};
		Tristeon::JsonSerializer::save(path.toStdString() + "/settings.tristeon", project);
		Tristeon::JsonSerializer::save(path.toStdString() + "/Scene.scene", Tristeon::Scene().serialize());

		loadProject(path.toStdString() + "/settings.tristeon");
	}

	void ProjectWindow::openProject()
	{
		QString const path = QFileDialog::getOpenFileName(this, QWidget::tr("Open Project File"),
		                                                  "C:/", QWidget::tr("Tristeon Project Files (*.tristeon)"));
		if (path.isEmpty() || !QFile::exists(path))
			return;
		
		loadProject(path.toStdString());
	}

	void ProjectWindow::projectClicked(Tristeon::String path)
	{
		Tristeon::Project::load(QFileInfo(QString::fromStdString(path)).dir().path().toStdString() + "/");
		close();
	}

	void ProjectWindow::loadProject(Tristeon::String path)
	{
		Tristeon::Project::load(QFileInfo(path.c_str()).dir().path().toStdString() + "/");

		json cache = Tristeon::JsonSerializer::load("Local/Projects.json");
		json::array_t projects = cache["projects"].get<json::array_t>();
		bool newProject = true;
		for (const auto& project : projects)
		{
			if (project.get<std::string>() == path)
				newProject = false;
		}

		if (newProject)
		{
			projects.push_back(path);
			cache["projects"] = projects;
			Tristeon::JsonSerializer::save("Local/Projects.json", cache);
		}

		close();
	}
}
#endif
