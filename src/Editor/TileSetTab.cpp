﻿#include "TileSetTab.h"

#include <qgridlayout.h>
#include <qlabel.h>


#include "Resources.h"
#include "Math/Vector2.h"

using namespace Tristeon;

TristeonEditor::TileSetTab::TileSetTab(QWidget* parent, const String& path) : QWidget(parent)
{
	//Setup grid layout
	auto* layout = new QGridLayout();
	setLayout(layout);

	//Load tileset
	tileSet = Resources::load<TileSet>(path);
	
	//Display tileset
	auto* image = new QLabel(this);
	layout->addWidget(image);

	image->show();
	image->setPixmap(QPixmap(path.c_str()).scaled(200, 200, Qt::AspectRatioMode::KeepAspectRatio));

	//Create transparent highlight
	auto* tileHighlight = new QLabel(image);
	tileHighlight->setPixmap(QPixmap("Internal/Textures/selection.png"));
	tileHighlight->setScaledContents(true);
	tileHighlight->setAttribute(Qt::WA_TranslucentBackground);

	//Scale highlight to tile size
	Vector2 const size = Vector2(image->width(), image->height()) * tileSet->tileSizeNormalized();
	tileHighlight->setMinimumSize((int)size.x, (int)size.y);
	tileHighlight->setMaximumSize((int)size.x, (int)size.y);
	tileHighlight->adjustSize();
	tileHighlight->hide();
	
	//image->setMaximumSize(image->pixmap()->width(), image->pixmap()->height());
	//image->adjustSize();
}
