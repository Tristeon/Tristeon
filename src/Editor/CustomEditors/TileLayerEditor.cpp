#include "Serialization/JsonSerializer.h"
#ifdef TRISTEON_EDITOR
#include "TileLayerEditor.h"

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::TileLayer, TileLayerEditor)

	void TileLayerEditor::initialize()
	{
		name = new QLabel("Tileset", this);
		button = new QPushButton("Load tileset", this);
		image = new QLabel();
		image->setPixmap(QPixmap(targetLayer->set()->texture->getPath().c_str()).scaled(250, 250, Qt::AspectRatioMode::KeepAspectRatio));
		image->setMaximumSize(250, 250);
		
		connect(button, &QPushButton::clicked, this, &TileLayerEditor::browse);
		auto* layout = new QVBoxLayout();
		layout->addWidget(name);
		layout->addWidget(button);
		layout->addWidget(image);
		setLayout(layout);

		name->show();
		button->show();
		image->show();
	}

	void TileLayerEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		targetLayer = dynamic_cast<Tristeon::TileLayer*>(current);
	}

	void TileLayerEditor::browse()
	{
		QDir const baseDir(QDir::currentPath());
		
		QString const path = QFileDialog::getOpenFileName(this, tr("Find Files"), QDir::currentPath() + "/Project", "*.tileset");
		QString const localPath = baseDir.relativeFilePath(path);

		if (path.isEmpty())
			return;
		
		Tristeon::TileSet* tileset = Tristeon::JsonSerializer::deserialize<Tristeon::TileSet>(localPath.toStdString());

		if (tileset != nullptr)
		{
			image->setPixmap(QPixmap(QString(tileset->texture->getPath().c_str())).scaled(250, 250, Qt::AspectRatioMode::KeepAspectRatio));

			int data[60]{
			13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
			4, 2, 3, 1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, 13, 13, 13, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			};
			
			json j = targetLayer->serialize();
			j["data"] = json::array_t(j["width"].get<int>() * j["height"].get<int>());
			for (int i = 0; i < j["width"].get<int>() * j["height"].get<int>(); i++)
				j["data"][i] = data[i];
			j["tileSet"] = tileset->serialize();
			targetLayer->deserialize(j);
		}
	}
}
#endif