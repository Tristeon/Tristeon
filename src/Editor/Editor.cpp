#include <qmessagebox.h>

#ifdef TRISTEON_EDITOR
#include "Project.h"
#include "Editor.h"
#include <Window.h>
#include <Scenes/SceneManager.h>

#include <qfiledialog.h>

namespace TristeonEditor
{
	Editor::Editor()
	{
		QString const path = QFileDialog::getOpenFileName(Tristeon::Window::instance(), QWidget::tr("Open Project File"),
		                                                  "C:/", QWidget::tr("Tristeon Project Files (*.tristeon)"));

		if (path.isEmpty() || !QFile::exists(path))
		{
			QMessageBox::warning(Tristeon::Window::instance(), QWidget::tr("Error"), QWidget::tr("No valid project file was selected!"), "Okay");
			Tristeon::Window::close();
		}

		Tristeon::Project::load(QFileInfo(path).dir().path().toStdString() + "/");
	}

	void Editor::initialize()
	{
		//Setup scene load callback
		Tristeon::SceneManager::sceneLoaded += [&](Tristeon::Scene * scene)
		{
			Tristeon::Window::instance()->setWindowTitle(QString::fromStdString("Tristeon2D | " + scene->getName()));
			
			selectedLayer(nullptr);
			for (auto window : windows)
				window->sceneLoaded(scene);
		};

		if (menuBar != nullptr)
			menuBar->initialize();
		
		//Initialize windows
		for (auto window : windows)
			window->initialize();
	}

	void Editor::addWindow(EditorWindow* window)
	{
		windows.add(window);
	}

	Tristeon::Layer* Editor::selectedLayer() const
	{
		return _selectedLayer;
	}

	void Editor::selectedLayer(Tristeon::Layer* value)
	{
		_selectedLayer = value;

		selectedActor(nullptr);
		
		for (auto window : windows)
			window->selectedLayerChanged(value);
	}

	Tristeon::Actor* Editor::selectedActor() const
	{
		return _selectedActor;
	}

	void Editor::selectedActor(Tristeon::Actor* value)
	{
		_selectedActor = value;

		for (auto window : windows)
			window->selectedActorChanged(value);
	}

	FileItem* Editor::selectedFileItem() const
	{
		return _selectedFileItem;
	}

	void Editor::selectedFileItem(FileItem* value)
	{
		_selectedFileItem = value;

		for (auto window : windows)
			window->selectedFilePathChanged(value);
	}

	void Editor::setMenuBar(MenuBar* menu_bar)
	{
		menuBar = menu_bar;
	}
}
#endif