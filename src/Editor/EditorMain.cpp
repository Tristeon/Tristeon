#ifdef TRISTEON_EDITOR
#include <GL/glew.h>
#include "Engine.h"
#include "AssetDatabase.h"

#include <QtWidgets>
#include <QtUiTools/QtUiTools>

#include <ctime>
#include "Project.h"

#include <Editor/Editor.h>
#include <Editor/EditorWindow.h>
#include <Editor/FileExplorer.h>
#include <Editor/GameView.h>
#include <Editor/LayerEditor.h>
#include <Editor/LayerListEditor.h>
#include <Editor/MenuBar.h>
#include <Editor/PropertyWindow.h>
#include <Editor/TileSetEditor.h>
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
		auto* tilesets = new TristeonEditor::TileSetEditor(parent);
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
	//Open project window to select a project
	QApplication projectApp(argc, argv);
	TristeonEditor::ProjectWindow projectWindow;
	QApplication::exec();

	if (Tristeon::Project::assetPath().empty())
		exit(0);
}

void showEditorWindow(int argc, char** argv)
{
	//Open Tristeon Editor
	QApplication app(argc, argv);
	TristeonEditor::EditorWindow window;
	window.setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	window.setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
	window.resize(1920, 1080);
	window.showMaximized();

	engine = std::make_unique<Tristeon::Engine>();
	editor = std::make_unique<TristeonEditor::Editor>();

	QWidget* widget = loadUIFile();
	window.setCentralWidget(widget);
	window.show();

	QApplication::processEvents();

	editor->initialize();
	engine->run();
}

int main(int argc, char** argv)
{
	srand(std::time(nullptr)); //TODO: Replace rand()

	showProjectWindow(argc, argv);

	Tristeon::AssetDatabase::load();

	showEditorWindow(argc, argv);
	return 0;
}
#endif