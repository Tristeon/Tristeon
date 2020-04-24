#include "Serialization/JsonSerializer.h"
#ifdef TRISTEON_EDITOR
#include "TileLayerEditor.h"

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::TileLayer, TileLayerEditor)

	void TileLayerEditor::initialize()
	{
		auto* layout = new QVBoxLayout();
		setLayout(layout);

		auto* formParent = new QWidget(this);
		formParent->show();
		layout->addWidget(formParent);
		
		auto* form = new QFormLayout();
		formParent->setLayout(form);
		
		//Width Field
		auto* widthText = new QLabel("Width", formParent);
		widthText->show();

		auto* widthField = new QSpinBox(formParent);
		widthField->setMinimum(1);
		widthField->setMaximum(std::numeric_limits<int>::max());
		widthField->setSingleStep(1);
		widthField->setValue(targetLayer->width());
		widthField->show();

		form->addRow(widthText, widthField);
		
		//Height Field
		auto* heightText = new QLabel("Height", formParent);
		heightText->show();

		auto* heightField = new QSpinBox(formParent);
		heightField->setMinimum(1);
		heightField->setMaximum(std::numeric_limits<int>::max());
		heightField->setSingleStep(1);
		heightField->setValue(targetLayer->width());
		heightField->show();

		form->addRow(heightText, heightField);

		//Tileset
		name = new QLabel("Tileset", formParent);
		name->show();

		button = new QPushButton("Load tileset", formParent);
		button->setStyleSheet("background-color: rgb(255, 255, 255);");
		button->show();

		form->addRow(name, button);
		connect(button, &QPushButton::clicked, this, &TileLayerEditor::browse);

		QFrame* frame = new QFrame(this);
		frame->setMinimumSize(250, 250);
		frame->setFrameStyle(QFrame::Box | QFrame::Raised);
		frame->setLineWidth(2);
		frame->show();
		layout->addWidget(frame);
		
		QGridLayout* frameLayout = new QGridLayout();
		frame->setLayout(frameLayout);

		image = new QLabel(frame);
		frameLayout->addWidget(image);
		image->show();
		image->setAlignment(Qt::AlignCenter);
		image->setPixmap(QPixmap(targetLayer->set()->texture->getPath().c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
		image->setMinimumSize(200, 200);
		image->setMaximumSize(200, 200);
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
			image->setPixmap(QPixmap(QString(tileset->texture->getPath().c_str())).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));

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