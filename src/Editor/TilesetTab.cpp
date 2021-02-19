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
	_tileset = Resources::jsonLoad<Tileset>(tilesetPath);
	_tilesetPath = tilesetPath;
	_selectedTile.tilesetID = _tileset->id;

	//Display tileset
	_image = new QLabel(this);
	_image->setAlignment(Qt::AlignCenter);
	_image->setPixmap(QPixmap((Settings::assetPath() + _tileset->albedoPath).c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));
	_image->setMaximumSize(_image->pixmap()->width(), _image->pixmap()->height());
	_image->adjustSize();

	//Tile selection highlight
	_tileHighlight = new QLabel(_image);
	_tileHighlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
	_tileHighlight->setScaledContents(true);
	_tileHighlight->setAttribute(Qt::WA_TranslucentBackground);

	//Scale highlight to tile size
	Vector const size = Vector(_image->width(), _image->height()) * _tileset->tileSizeNormalized();
	_tileHighlight->setMinimumSize((int)size.x, (int)size.y);
	_tileHighlight->setMaximumSize((int)size.x, (int)size.y);
	_tileHighlight->adjustSize();
	_tileHighlight->hide();

	_image->setMaximumSize(_image->pixmap()->width(), _image->pixmap()->height());
	_image->adjustSize();

	//Setup grid layout
	_layout = new QVBoxLayout();
	_layout->addWidget(_image);
	setLayout(_layout);
}

void TristeonEditor::TilesetTab::mousePressEvent(QMouseEvent* event)
{
	QPoint const mousePos = event->globalPos();
	QRect rect = _image->geometry();
	rect.moveTopLeft(_image->parentWidget()->mapToGlobal(rect.topLeft()));

	if (rect.contains(mousePos))
	{
		//Calculate tile index the mouse is hovering over
		VectorI const size = VectorI(_image->width(), _image->height());
		VectorI const tileSize = size / VectorI(_tileset->cols, _tileset->rows);

		QPoint local = _image->mapFromGlobal(mousePos);

		VectorI tileIndex = VectorI(local.x() / tileSize.x, local.y() / tileSize.y);
		int const newTile = _tileset->tile(tileIndex);

		setSelectedTile(newTile);
	}
	else
	{
		clearSelection();
	}
}

void TristeonEditor::TilesetTab::setSelectedTile(const int tileIndex)
{
	if (_selectedTile.index == tileIndex) //Unselect
	{
		clearSelection();
	}
	else
	{
		_selectedTile.index = tileIndex;

		//Move highlight
		Vector const tilePos = _tileset->tileMinNormalized(_selectedTile.index) * Vector(_image->width(), _image->height());
		_tileHighlight->move((int)tilePos.x, (int)tilePos.y);
		_tileHighlight->show();

		Brushes::selectTile(_selectedTile, _tilesetPath);
	}
}

void TristeonEditor::TilesetTab::clearSelection()
{
	_selectedTile.index = -1;
	_tileHighlight->hide();
}

#endif