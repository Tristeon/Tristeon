#include "Rendering/Shader.h"
#ifdef TRISTEON_EDITOR
#include "Settings.h"
#include "Animations/AnimationClip.h"
#include "AssetManagement/AssetDatabase.h"
#include "Serialization/JsonSerializer.h"
#include "MenuBar.h"
#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>
#include <Scenes/Tiles/Tileset.h>

#include <QtWidgets>

namespace TristeonEditor
{
	void MenuBar::initialize()
	{
		auto* create = findChild<QMenu*>("menuCreate");
		auto* open = findChild<QMenu*>("menuOpen");
		auto* save = findChild<QMenu*>("menuSave");

		auto* createScene = findAction(create, "createScene");
		connect(createScene, &QAction::triggered, this, &MenuBar::createScene);

		auto* createTileset = findAction(create, "createTileset");
		connect(createTileset, &QAction::triggered, this, &MenuBar::createTileset);

		auto* createAnimationClip = findAction(create, "createAnimationClip");
		connect(createAnimationClip, &QAction::triggered, this, &MenuBar::createAnimationClip);

		auto* openScene = findAction(open, "openScene");
		connect(openScene, &QAction::triggered, this, &MenuBar::openScene);
		
		auto* saveScene = findAction(save, "saveScene");
		connect(saveScene, &QAction::triggered, this, &MenuBar::saveScene);

		auto* recompileShaders = findAction(findChild<QMenu*>("menuAction"), "actionRecompile_Shaders");
		connect(recompileShaders, &QAction::triggered, this, []()
		{
			for (auto* shader : Tristeon::Collector<Tristeon::Shader>::all())
				shader->reload();
		});
	}

	void MenuBar::createScene()
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getSaveFileName(this, tr("Find Files"), Tristeon::Settings::assetPath().c_str(), "*.scene");
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;
		
		auto* scene = new Tristeon::Scene();
		Tristeon::SceneManager::save(scene, localPath.toStdString());

		saveCurrentSceneDialog(fileName.toStdString());
	}

	void MenuBar::createTileset()
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getSaveFileName(this, tr("Find Files"), Tristeon::Settings::assetPath().c_str(), "*.tileset");
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;

		Tristeon::Tileset tileset{};
		json j = tileset.serialize();
		j["id"] = Tristeon::Random::generateUInt();
		Tristeon::JsonSerializer::save(path.toStdString(), j);
		Tristeon::AssetDatabase::add(localPath.toStdString());
	}

	void MenuBar::createAnimationClip()
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getSaveFileName(this, tr("Find Files"), Tristeon::Settings::assetPath().c_str(), "*.clip");
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;

		Tristeon::AnimationClip anim{};
		json j = anim.serialize();
		Tristeon::JsonSerializer::save(path.toStdString(), j);
		Tristeon::AssetDatabase::add(localPath.toStdString());
	}

	void MenuBar::openScene()
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getOpenFileName(this, tr("Find Files"), Tristeon::Settings::assetPath().c_str(), "*.scene");
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;

		saveCurrentSceneDialog(fileName.toStdString());
	}

	void MenuBar::saveScene()
	{
		if (Tristeon::SceneManager::current()->path().empty())
		{
			QDir const baseDir(Tristeon::Settings::assetPath().c_str());

			QString const path = QFileDialog::getSaveFileName(this, tr("Find Files"), Tristeon::Settings::assetPath().c_str(), "*.scene");
			QString const localPath = baseDir.relativeFilePath(path);
			QString const fileName = QFileInfo(path).baseName();
			if (path.isEmpty() || localPath.isEmpty())
			{
				TRISTEON_WARNING("No path was selected for saving Scene, therefore the scene wasn't saved!");
				return;
			}
			Tristeon::SceneManager::save(Tristeon::SceneManager::current(), path.toStdString());
		}
		else
		{
			Tristeon::SceneManager::saveCurrent();
		}
	}

	void MenuBar::saveCurrentSceneDialog(std::string const nextSceneName)
	{
		auto const box = QMessageBox::information(this, tr("Tristeon2D"), tr("Save scene changes?"), "Yes", "No");

		if (!box)
			Tristeon::SceneManager::saveCurrent();

		Tristeon::SceneManager::load(nextSceneName);
	}

	QAction* MenuBar::findAction(QMenu* menu, Tristeon::String name)
	{
		for (auto* action : menu->actions())
		{
			if (action->objectName() == QString::fromStdString(name))
				return action;
		}
		return nullptr;
	}
}
#endif