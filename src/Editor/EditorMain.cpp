#ifdef TRISTEON_EDITOR
#include <glad/glad.h>
#include "Engine.h"
#include "AssetManagement/AssetDatabase.h"

#include <QtWidgets>
#include <QtUiTools/QtUiTools>

#include <ctime>
#include "Project.h"
#include <Utils/Console.h>

#include <Editor/Editor.h>
#include <Editor/EditorWindow.h>
#include <Editor/AssetExplorer/FileExplorer.h>
#include <Editor/GameView.h>
#include <Editor/LayerEditor.h>
#include <Editor/LayerListEditor.h>
#include <Editor/MenuBar.h>
#include <Editor/PropertyWindow.h>
#include <Editor/TilesetEditor.h>
#include <Editor/TopBar.h>
#include <Editor/QtPlugins/AspectRatioWidget.h>
#include "Editor/ProjectWindow.h"

std::unique_ptr<TristeonEditor::Editor> editor;
std::unique_ptr<Tristeon::Engine> engine;

class CustomLoader : public QUiLoader
{
	QWidget* createWidget(const QString& className, QWidget* parent = nullptr, const QString& name = QString()) override;
};

QWidget* CustomLoader::createWidget(const QString& className, QWidget* parent, const QString& name)
{
	if (name == "MainWindow")
	{
		auto* window = new TristeonEditor::EditorWindow();
		return window;
	}
	
	if (name == "game")
	{
		auto* gameView = new TristeonEditor::GameView(parent);
		editor->addWidget(gameView);
		auto* aspect_ratio = new TristeonEditor::AspectRatioWidget(gameView, parent->width(), parent->height(), parent);
		return aspect_ratio;
	}

	if (name == "topbar")
	{
		auto* topBar = new TristeonEditor::TopBar(parent);
		editor->addWidget(topBar);
		return topBar;
	}

	if (name == "layers")
	{
		auto* layerList = new TristeonEditor::LayerListEditor(parent);
		editor->addWidget(layerList);
		return layerList;
	}

	if (name == "layereditor")
	{
		auto* layerEditor = new TristeonEditor::LayerEditor(parent);
		editor->addWidget(layerEditor);
		return layerEditor;
	}

	if (name == "properties")
	{
		auto* properties = new TristeonEditor::PropertyWindow(parent);
		editor->addWidget(properties);
		return properties;
	}

	if (name == "menubar")
	{
		auto* menuBar = new TristeonEditor::MenuBar(parent);
		editor->setMenuBar(menuBar);
		return menuBar;
	}

	if (name == "fileexplorer")
	{
		auto* explorer = new TristeonEditor::FileExplorer(parent);
		editor->addWidget(explorer);
		return explorer;
	}

	if (name == "tilesets")
	{
		auto* tilesets = new TristeonEditor::TilesetEditor(parent);
		editor->addWidget(tilesets);
		return tilesets;
	}

	return QUiLoader::createWidget(className, parent, name);
}

QWidget* loadUIFile()
{
	CustomLoader loader;
	QFile file("Editor.ui");
	file.open(QFile::ReadOnly);

	QWidget* formWidget = loader.load(&file);
	file.close();

	return formWidget;
}

void showProjectWindow(int argc, char** argv)
{
	QApplication projectApp(argc, argv);
	TristeonEditor::ProjectWindow projectWindow;
	QApplication::exec();

	if (Tristeon::Project::assetPath().empty())
		exit(0);
}

int main(int argc, char** argv)
{
	Tristeon::Console::init();
	
	showProjectWindow(argc, argv);

	Tristeon::AssetDatabase::load();

	//Run editor
	engine = std::make_unique<Tristeon::Engine>();
	editor = std::make_unique<TristeonEditor::Editor>();

	QApplication app(argc, argv);
	QWidget* widget = loadUIFile();
	auto* window = static_cast<TristeonEditor::EditorWindow*>(widget);
	window->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	window->resize(1920, 1080);
	window->showMaximized();

	QSettings settings("Tristeon", "Tristeon2D");
	window->restoreGeometry(settings.value("geometry").toByteArray());
	window->restoreState(settings.value("windowState").toByteArray());

	window->show();

	QApplication::processEvents();

	editor->initialize();
	engine->run();
	return 0;
}
#endif