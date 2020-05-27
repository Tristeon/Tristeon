#ifdef TRISTEON_EDITOR

#include "TileSetTab.h"

#include <qformlayout.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QMouseEvent>

#include "Resources.h"
#include "Brushes/Brushes.h"
#include "Math/Vector2.h"

using namespace Tristeon;

TristeonEditor::TileSetTab::TileSetTab(const String& tileSetPath)
{
	//Load tileset
	tileSet = Resources::jsonLoad<TileSet>(tileSetPath);
	selectedTile.tileSetID = tileSet->id;
	
	//Display tileset
	image = new QLabel(this);
	image->setAlignment(Qt::AlignCenter);
	image->setPixmap(QPixmap(tileSet->texture->getPath().c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
	image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
	image->adjustSize();

	//Tile selection highlight
	tileHighlight = new QLabel(image);
	tileHighlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
	tileHighlight->setScaledContents(true);
	tileHighlight->setAttribute(Qt::WA_TranslucentBackground);

	//Scale highlight to tile size
	Vector2 const size = Vector2(image->width(), image->height()) * tileSet->tileSizeNormalized();
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

void TristeonEditor::TileSetTab::mousePressEvent(QMouseEvent* event)
{
	QPoint const mousePos = event->globalPos();
	QRect rect = image->geometry();
	rect.moveTopLeft(image->parentWidget()->mapToGlobal(rect.topLeft()));

	if (rect.contains(mousePos))
	{
		//Calculate tile index the mouse is hovering over
		Vector2Int const size = Vector2Int(image->width(), image->height());
		Vector2Int const tileSize = size / Vector2Int(tileSet->cols, tileSet->rows);

		QPoint local = image->mapFromGlobal(mousePos);

		Vector2Int tileIndex = Vector2Int(local.x() / tileSize.x, local.y() / tileSize.y);
		int const newTile = tileSet->tile(tileIndex);

		setSelectedTile(newTile);
	}
	else
	{
		clearSelection();
	}
}

void TristeonEditor::TileSetTab::setSelectedTile(const int tileIndex)
{
	if (selectedTile.index == tileIndex) //Unselect
	{
		clearSelection();
	}
	else
	{
		selectedTile.index = tileIndex;

		//Move highlight
		Vector2 const tilePos = tileSet->tileMinNormalized(selectedTile.index) * Vector2(image->width(), image->height());
		tileHighlight->move((int)tilePos.x, (int)tilePos.y);
		tileHighlight->show();

		Brushes::selectTile(selectedTile);
	}
}

void TristeonEditor::TileSetTab::clearSelection()
{
	selectedTile.index = -1;
	tileHighlight->hide();
}

#endif