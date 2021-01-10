#ifdef TRISTEON_EDITOR

#include "TilesetTab.h"

#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QMouseEvent>

#include "AssetManagement/Resources.h"
#include "Brushes/Brushes.h"
#include "Math/Vector.h"

using namespace Tristeon;

TristeonEditor::TilesetTab::TilesetTab(const String& tilesetPath)
{
	//Load tileset
	tileset = Resources::jsonLoad<Tileset>(tilesetPath);
	tileset->filePath = tilesetPath;
	selectedTile.tilesetID = tileset->id;
	
	//Display tileset
	image = new QLabel(this);
	image->setAlignment(Qt::AlignCenter);
	image->setPixmap(QPixmap((Settings::assetPath() + tileset->texturePath).c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
	image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
	image->adjustSize();

	//Tile selection highlight
	tileHighlight = new QLabel(image);
	tileHighlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
	tileHighlight->setScaledContents(true);
	tileHighlight->setAttribute(Qt::WA_TranslucentBackground);

	//Scale highlight to tile size
	Vector const size = Vector(image->width(), image->height()) * tileset->tileSizeNormalized();
	tileHighlight->setMinimumSize((int)size.x, (int)size.y);
	tileHighlight->setMaximumSize((int)size.x, (int)size.y);
	tileHighlight->adjustSize();
	tileHighlight->hide();

	image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
	image->adjustSize();

	//Setup grid layout
	layout = new QVBoxLayout();
	layout->addWidget(image);
	setLayout(layout);
}

void TristeonEditor::TilesetTab::mousePressEvent(QMouseEvent* event)
{
	QPoint const mousePos = event->globalPos();
	QRect rect = image->geometry();
	rect.moveTopLeft(image->parentWidget()->mapToGlobal(rect.topLeft()));

	if (rect.contains(mousePos))
	{
		//Calculate tile index the mouse is hovering over
		VectorI const size = VectorI(image->width(), image->height());
		VectorI const tileSize = size / VectorI(tileset->cols, tileset->rows);

		QPoint local = image->mapFromGlobal(mousePos);

		VectorI tileIndex = VectorI(local.x() / tileSize.x, local.y() / tileSize.y);
		int const newTile = tileset->tile(tileIndex);

		setSelectedTile(newTile);
	}
	else
	{
		clearSelection();
	}
}

void TristeonEditor::TilesetTab::setSelectedTile(const int tileIndex)
{
	if (selectedTile.index == tileIndex) //Unselect
	{
		clearSelection();
	}
	else
	{
		selectedTile.index = tileIndex;

		//Move highlight
		Vector const tilePos = tileset->tileMinNormalized(selectedTile.index) * Vector(image->width(), image->height());
		tileHighlight->move((int)tilePos.x, (int)tilePos.y);
		tileHighlight->show();

		Brushes::selectTile(selectedTile, tileset);
	}
}

void TristeonEditor::TilesetTab::clearSelection()
{
	selectedTile.index = -1;
	tileHighlight->hide();
}

#endif