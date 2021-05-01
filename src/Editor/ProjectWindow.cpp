#ifdef TRISTEON_EDITOR
#include "Palette.h"
#include "EditorFields.h"
#include "Scenes/Scene.h"
#include "Settings.h"
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

		auto* contents = findChild<QWidget*>("settings_contents");
		contents->hide();
	}

	void ProjectWindow::mousePressEvent(QMouseEvent* event)
	{
		for (auto const& projectWidget : projectWidgets)
		{
			if (!projectWidget->isVisible())
				continue;

			if (projectWidget->underMouse() && event->button() == Qt::LeftButton)
				loadSettingsWidget(projectWidget);
		}
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
				projectWidget->setStyleSheet(Palette::getRGBString("background-color", Palette::background));
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

		json cache = Tristeon::JsonSerializer::load("Local://Projects.json");

		if (cache.is_null())
		{
			cache = json();
			cache["projects"] = json::array_t();
			Tristeon::JsonSerializer::save("Local://Projects.json", cache);
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

			projectWidgets.add(projectWidget);

			auto* title = projectWidget->findChild<QLabel*>("title");
			title->setText(dir.dirName());

			auto* pathLabel = projectWidget->findChild<QLabel*>("path");
			pathLabel->setText(QString::fromStdString(path));
		}

		json save;
		save["projects"] = valid_projects;
		Tristeon::JsonSerializer::save("Local://Projects.json", save);

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

		json const project = Tristeon::Settings::serialize();
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
		Tristeon::Settings::load(QFileInfo(QString::fromStdString(path)).dir().path().toStdString() + "/");
		close();
	}

	void ProjectWindow::loadProject(Tristeon::String path)
	{
		Tristeon::Settings::load(QFileInfo(path.c_str()).dir().path().toStdString() + "/");

		json cache = Tristeon::JsonSerializer::load("Local://Projects.json");
		json::array_t projects = cache.is_null() ? json::array_t() : cache.value("projects", json::array_t());
		if (cache.is_null())
			cache = json{};
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
			Tristeon::JsonSerializer::save("Local://Projects.json", cache);
		}

		close();
	}

	void ProjectWindow::loadSettingsWidget(QWidget* projectWidget)
	{
		if (selectedProject != nullptr)
			selectedProject->findChild<QWidget*>("frame")->setStyleSheet("");
		selectedProject = projectWidget;

		selectedProject->findChild<QWidget*>("frame")->setStyleSheet(Palette::getRGBString("background-color", Palette::background));
		
		//Load current settings
		Tristeon::String const path = projectWidget->objectName().toStdString();
		json const settings = Tristeon::JsonSerializer::load(path);

		//Disconnect and clear all active connections
		for (auto const& connection : activeConnections)
			disconnect(connection);
		activeConnections.clear();
		
		auto* contents = findChild<QWidget*>("settings_contents");
		contents->show();

		//Scene
		auto* scene_field = findChild<QLineEdit*>("scene_field");
		scene_field->setText(QString::fromStdString(settings.value("firstScene", "")));
		activeConnections.add(connect(scene_field, &QLineEdit::textChanged, [=](const QString & val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["firstScene"] = val.toStdString();
				Tristeon::JsonSerializer::save(path, file);
			}));

		//Graphics
		auto graphics = settings.value("graphics", json::object());
		
		auto* tile_width_field = findChild<QSpinBox*>("tile_width_field");
		tile_width_field->setValue(graphics.value("tileWidth", 64u));
		activeConnections.add(connect(tile_width_field, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int & val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["tileWidth"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto* tile_height_field = findChild<QSpinBox*>("tile_height_field");
		tile_height_field->setValue(graphics.value("tileHeight", 64u));
		activeConnections.add(connect(tile_height_field, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["tileHeight"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto* vsync_field = findChild<QCheckBox*>("vsync_field");
		vsync_field->setChecked(graphics.value("vsync", false));
		activeConnections.add(connect(vsync_field, &QCheckBox::stateChanged, [=](const int& state)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["vsync"] = ((Qt::CheckState)state != Qt::Unchecked);
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto* windowmode_field = findChild<QComboBox*>("windowmode_field");
		windowmode_field->setCurrentIndex(graphics.value("windowMode", 2));
		activeConnections.add(connect(windowmode_field, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](const int& index)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["windowMode"] = index;
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto* max_fps_field = findChild<QSpinBox*>("fps_field");
		max_fps_field->setValue(graphics.value("maxFPS", 0));
		activeConnections.add(connect(max_fps_field, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["maxFPS"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto * resolution_field_width = findChild<QSpinBox*>("resolution_field_width");
		resolution_field_width->setValue(graphics.value("preferredResolution", json::object()).value("x", 0));
		activeConnections.add(connect(resolution_field_width, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["preferredResolution"]["x"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));

		auto * resolution_field_height = findChild<QSpinBox*>("resolution_field_height");
		resolution_field_height->setValue(graphics.value("preferredResolution", json::object()).value("y", 0));
		activeConnections.add(connect(resolution_field_height, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["graphics"]["preferredResolution"]["y"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));

		//Physics
		auto physics = settings.value("physics", json::object());

		auto* fixed_delta_time_field = findChild<QDoubleSpinBox*>("fixed_delta_time_field");
		fixed_delta_time_field->setValue(physics.value("fixedDeltaTime", 1000.0f / 50.0f));
		activeConnections.add(connect(fixed_delta_time_field, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](const double& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["physics"]["fixedDeltaTime"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));
		
		auto* pixels_per_meter_field = findChild<QSpinBox*>("pixels_per_meter_field");
		pixels_per_meter_field->setValue(physics.value("pixelsPerMeter", 64));
		activeConnections.add(connect(pixels_per_meter_field, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](const int& val)
			{
				json file = Tristeon::JsonSerializer::load(path);
				file["physics"]["pixelsPerMeter"] = val;
				Tristeon::JsonSerializer::save(path, file);
			}));
	}
}
#endif
