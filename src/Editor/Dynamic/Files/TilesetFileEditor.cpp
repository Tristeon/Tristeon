#ifdef TRISTEON_EDITOR
#include <AssetManagement/Resources.h>
#include "TilesetFileEditor.h"
#include "Editor/EditorFields.h"
#include "Scenes/Tiles/Tileset.h"

using Tristeon::Vector;
using Tristeon::VectorI;

namespace TristeonEditor
{
	void TilesetFileEditor::initialize()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		EditorFields::uintField(form, "Columns", data["width"], 1, std::numeric_limits<int>::max(), [&](uint value) { data["width"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(form, "Rows", data["height"], 1, std::numeric_limits<int>::max(), [&](uint value) { data["height"] = value; saveData(); loadTileset(); });
		
		auto* spacing = new QWidget(formWidget);
		layout->addWidget(spacing);
		auto* spacingLayout = new QFormLayout(spacing);
		spacing->setLayout(spacingLayout);

		EditorFields::uintField(spacingLayout, "Left", data["spacingLeft"], [&](uint value) { data["spacingLeft"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(spacingLayout, "Right", data["spacingRight"], [&](uint value) { data["spacingRight"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(spacingLayout, "Top", data["spacingTop"], [&](uint value) { data["spacingTop"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(spacingLayout, "Bottom", data["spacingBottom"], [&](uint value) { data["spacingBottom"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(spacingLayout, "Horizontal", data["horizontalSpacing"], [&](uint value) { data["horizontalSpacing"] = value; saveData(); loadTileset(); });
		EditorFields::uintField(spacingLayout, "Vertical", data["verticalSpacing"], [&](uint value) { data["verticalSpacing"] = value; saveData(); loadTileset(); });
		form->addRow(new QLabel("Spacing"), spacing);

		layout->addWidget(new QLabel("Albedo"));
		auto* changeAlbedo = new QPushButton(data.value("albedoPath", "").c_str(), this);
		layout->addWidget(changeAlbedo);
		connect(changeAlbedo, &QPushButton::clicked, this, [=]() { changeTexturePressed("albedoPath", changeAlbedo); });

		layout->addWidget(new QLabel("Normal"));
		auto* changeNormal = new QPushButton(data.value("normalPath", "").c_str(), this);
		layout->addWidget(changeNormal);
		connect(changeNormal, &QPushButton::clicked, this, [=]() { changeTexturePressed("normalPath", changeNormal); });

		EditorFields::floatField(form, "Normal map strength", data.value("normalMapStrength", 1.0f), [&](float value) { data["normalMapStrength"] = value; saveData(); loadTileset(); });

		layout->addWidget(new QLabel("Light Mask"));
		auto* changeLightMask = new QPushButton(data.value("lightMaskPath", "").c_str(), this);
		layout->addWidget(changeLightMask);
		connect(changeLightMask, &QPushButton::clicked, this, [=]() { changeTexturePressed("lightMaskPath", changeLightMask); });
		
		//Tile Editing
		auto* frame = new QFrame(this);
		frame->setMinimumSize(250, 250);
		frame->setFrameStyle(QFrame::Box | QFrame::Raised);
		frame->setLineWidth(2);
		frame->show();
		layout->addWidget(frame);

		auto * frameLayout = new QGridLayout();
		frame->setLayout(frameLayout);

		image = new QLabel(frame);
		frameLayout->addWidget(image);
		image->show();
		image->setAlignment(Qt::AlignCenter);
		image->setPixmap(QPixmap((Tristeon::Settings::assetPath() + data.value("albedoPath", std::string())).c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
		image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
		image->adjustSize();

		tileHighlight = new QLabel(image);
		tileHighlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
		tileHighlight->setScaledContents(true);
		tileHighlight->setAttribute(Qt::WA_TranslucentBackground);
		tileHighlight->setFocusPolicy(Qt::NoFocus);

		loadTileset();
	}

	void TilesetFileEditor::changeTexturePressed(const Tristeon::String& idx, QPushButton* button)
	{
		QDir const baseDir(Tristeon::Settings::assetPath().c_str());

		QString const path = QFileDialog::getOpenFileName(this, tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), tr("Image Files (*.png *.jpg *.bmp)"));
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();

		data[idx] = localPath.toStdString();
		saveData();
		loadTileset();

		button->setText(localPath);
	}

	void TilesetFileEditor::saveData()
	{
		JsonFileEditor::saveData();

		//Update Instance if it's currently in use
		if (Tristeon::Resources::loaded(item->path))
		{
			auto* tileset = Tristeon::Resources::jsonLoad<Tristeon::Tileset>(item->path);
			if (tileset != nullptr)
				tileset->deserialize(data);
		}
	}

	void TilesetFileEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		JsonFileEditor::targetChanged(current, old);
		set = Tristeon::Resources::jsonLoad<Tristeon::Tileset>(item->path);
		data = set->serialize();
	}

	void TilesetFileEditor::loadTileset()
	{
		tilesetChanged();

		selectedTile = -1;
		selectedTileChanged();

		image->setPixmap(QPixmap((Tristeon::Settings::assetPath() + data.value("albedoPath", std::string())).c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
		image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
		image->adjustSize();
	}

	void TilesetFileEditor::mousePressEvent(QMouseEvent* event)
	{
		QPoint const position = event->globalPos();
		QRect rect = image->geometry();
		rect.moveTopLeft(image->parentWidget()->mapToGlobal(rect.topLeft()));

		if (rect.contains(position))
		{
			VectorI const size = VectorI(image->width(), image->height());
			VectorI const tileSize = size / VectorI(data["width"], data["height"]);

			QPoint const local = image->mapFromGlobal(position);

			VectorI const tileIndex = VectorI(local.x() / tileSize.x, local.y() / tileSize.y);
			int const newTile = set->tile(tileIndex.x, tileIndex.y);
			if (newTile == selectedTile)
			{
				selectedTile = -1;
				tileHighlight->hide();
			}
			else
			{
				selectedTile = newTile;
				Vector const tilePos = set->tileMinNormalized(newTile) * Vector(image->width(), image->height());
				tileHighlight->move((int)tilePos.x, (int)tilePos.y);
				tileHighlight->show();
			}
		}
		else
		{
			tileHighlight->hide();
			selectedTile = -1;
		}

		selectedTileChanged();
	}

	void TilesetFileEditor::selectedTileChanged()
	{
		if (selectedTileData != nullptr)
		{
			layout->removeWidget(selectedTileData);
			selectedTileData->deleteLater();
			selectedTileData->hide();
			selectedTileData = nullptr;
		}

		if (selectedTile == -1)
			return;

		selectedTileData = new QFrame(this);
		layout->addWidget(selectedTileData);

		selectedTileData->setFrameStyle(QFrame::Box | QFrame::Raised);
		selectedTileData->setLineWidth(2);
		auto* frameLayout = new QFormLayout(selectedTileData);
		selectedTileData->setLayout(frameLayout);

		selectedTileInfo = set->info(selectedTile);

		EditorFields::boolField(frameLayout, "Has Collider", selectedTileInfo.hasCollider, [&](bool value) { selectedTileInfo.hasCollider = value; saveCurrentTile(); });
		EditorFields::floatField(frameLayout, "Density", selectedTileInfo.density, [&](float value) { selectedTileInfo.density = value; saveCurrentTile(); });
		EditorFields::floatField(frameLayout, "Friction", selectedTileInfo.friction, [&](float value) { selectedTileInfo.friction = value; saveCurrentTile(); });
		EditorFields::floatField(frameLayout, "Restitution", selectedTileInfo.restitution, [&](float value) { selectedTileInfo.restitution = value; saveCurrentTile(); });
	}

	void TilesetFileEditor::saveCurrentTile()
	{
		data["tileInfo"][selectedTile] = selectedTileInfo;
		saveData();
	}

	void TilesetFileEditor::tilesetChanged()
	{
		set->deserialize(data);

		Vector const size = set->tileSizeNormalized() * Tristeon::Vector(image->width(), image->height());
		tileHighlight->setMinimumSize((int)size.x, (int)size.y);
		tileHighlight->setMaximumSize((int)size.x, (int)size.y);
		tileHighlight->adjustSize();
		tileHighlight->hide();
	}
}
#endif