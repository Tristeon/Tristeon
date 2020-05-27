/*
	MIT License

	Copyright (c) 2020 Tristan Metz, Leon Brands

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "Engine.h"
#include "Window.h"

#ifndef TRISTEON_LOGENABLED
#include <Windows.h>
#endif

#include <QtWidgets>
#include <QtUiTools/QtUiTools>

#include <ctime>

#ifdef TRISTEON_EDITOR
#include <Editor/Editor.h>
#include <Editor/LayerListEditor.h>
#include <Editor/LayerEditor.h>
#include <Editor/QtPlugins/AspectRatioWidget.h>
#include <Editor/MenuBar.h>
#include <Editor/TopBar.h>
#include <Editor/PropertyWindow.h>
#include <Editor/FileExplorer.h>
#include <Editor/GameViewEditor.h>
#include <Editor/TileSetEditor.h>

std::unique_ptr<TristeonEditor::Editor> editor;
#endif

std::unique_ptr<Tristeon::Engine> engine;

#ifdef TRISTEON_EDITOR
class CustomLoader : public QUiLoader
{
	QWidget* createWidget(const QString& className, QWidget* parent = nullptr, const QString& name = QString()) override;
};

QWidget* CustomLoader::createWidget(const QString& className, QWidget* parent, const QString& name)
{
	if (name == "game")
	{
		auto* gameView = new TristeonEditor::GameViewEditor(engine.get(), parent); //Qt Child, no ownership
		editor->addWindow(gameView);
		auto* aspect_ratio = new TristeonEditor::AspectRatioWidget(gameView->widget(), parent->width(), parent->height(), parent);
		return aspect_ratio;
	}

	if (name == "topbar")
	{
		auto* topBar = new TristeonEditor::TopBar(parent);
		editor->addWindow(topBar);
		return topBar;
	}
	
	if (name == "layers")
	{
		auto* layerList = new TristeonEditor::LayerListEditor(parent);
		editor->addWindow(layerList);
		return layerList;
	}

	if (name == "layereditor")
	{
		auto* layerEditor = new TristeonEditor::LayerEditor(parent);
		editor->addWindow(layerEditor);
		return layerEditor;
	}

	if (name == "properties")
	{
		auto* properties = new TristeonEditor::PropertyWindow(parent);
		editor->addWindow(properties);
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
		editor->addWindow(explorer);
		return explorer;
	}

	if (name == "tilesets")
	{
		auto* tilesets = new TristeonEditor::TileSetEditor(parent);
		editor->addWindow(tilesets);
		return tilesets;
	}
	
	return QUiLoader::createWidget(className, parent, name);
}
#else
class CustomLoader : public QUiLoader
{
	QWidget* createWidget(const QString& className, QWidget* parent = nullptr, const QString& name = QString()) override;
};

QWidget* CustomLoader::createWidget(const QString& className, QWidget* parent, const QString& name)
{
	if (name == "game")
	{
		auto* gameView = new Tristeon::GameView(engine.get(), parent);
		return gameView->widget(); //Converts GameView into a QOpenGLWidget because we don't normally allow access to its base class 
	}
	return QUiLoader::createWidget(className, parent, name);
}
#endif

QWidget* loadUIFile()
{
	CustomLoader loader;
#if TRISTEON_EDITOR
	QFile file("Editor.ui");
#else
	QFile file("Game.ui");
#endif
	file.open(QFile::ReadOnly);
	
	QWidget* formWidget = loader.load(&file);
	file.close();

	return formWidget;
}

int main(int argc, char** argv)
{
	srand(std::time(nullptr));
	
#ifndef TRISTEON_LOGENABLED
	FreeConsole();
#endif

	QApplication app(argc, argv);

	Tristeon::Window window;
	window.setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
	window.setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
	window.resize(1920, 1080);
	window.showMaximized();
	engine = std::make_unique<Tristeon::Engine>();

#ifdef TRISTEON_EDITOR
	editor = std::make_unique<TristeonEditor::Editor>();
#endif
	
	QWidget* widget = loadUIFile();
	window.setCentralWidget(widget);
	window.show();

	//TODO: Docking doesn't respect corner wishes
	//for (QDockWidget* dock : widget->findChildren<QDockWidget*>())
	//{
	//	dock->setParent(&window);
	//	window.removeDockWidget(dock);
	//	window.addDockWidget(Qt::RightDockWidgetArea, dock);
	//	dock->show();
	//}

	QApplication::processEvents();
#ifdef TRISTEON_EDITOR
	editor->initialize();
#else
	window.showFullScreen();
	engine->playMode(true);
#endif
	engine->run();
	return 0;
}