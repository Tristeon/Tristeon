#include "Input/Keyboard.h"
#ifdef TRISTEON_EDITOR
#include "Actors/Sprite.h"
#include "Engine.h"
#include "ActorLayerSceneView.h"
#include "Rendering/GameView.h"
#include "Rendering/Graphic.h"
#include "Editor/Editor.h"
#include <Input/Mouse.h>

using namespace Tristeon;
namespace TristeonEditor
{
	SCENE_EDITOR_CPP(Tristeon::ActorLayer, ActorLayerSceneView);
	
	void ActorLayerSceneView::initialize()
	{
		actorLayer = dynamic_cast<ActorLayer*>(target());

		highlight = new QLabel(this);
		highlight->setPixmap(QPixmap("Internal/Textures/outline.png"));
		highlight->setScaledContents(true);
		highlight->setAttribute(Qt::WA_TranslucentBackground);
		highlight->hide();

		corner = new QLabel(this);
		corner->setPixmap(QPixmap("Internal/Textures/selection.png"));
		corner->setScaledContents(true);
		corner->hide();
	}

	void ActorLayerSceneView::updateView()
	{
		if (Engine::playMode())
		{
			highlight->hide();
			corner->hide();

			dragging = false;
			draggingCorner = false;
			return;
		}

		if (Mouse::pressed(Mouse::Left))
		{
			if (corner->isHidden() || !corner->underMouse())
				clickActor();
			else if (!corner->isHidden() && corner->underMouse())
			{
				draggingCorner = true;
			}
		}
		else if (Mouse::held(Mouse::Left))
		{
			if (dragging && editor()->selectedActor() != nullptr)
				editor()->selectedActor()->position = GameView::screenToWorld(Mouse::position());
			if (draggingCorner && editor()->selectedActor() != nullptr)
			{
				Sprite* sprite = dynamic_cast<Sprite*>(editor()->selectedActor());

				if (sprite != nullptr && sprite->getTexture() != nullptr)
				{
					Vector2 position = editor()->selectedActor()->position;
					Vector2 topRight = GameView::screenToWorld(Mouse::position());
					Vector2 difference = topRight - position;
					if (difference.x < 0 || difference.y < 0)
						return;

					Vector2 size = difference / sprite->scale * 2;

					//Snap to aspect ratio
					if (Keyboard::held(Keyboard::Shift))
					{
						Vector2 imageSize = sprite->getTexture()->size();
						
						if (size.x > size.y) //Prioritize X
							size.x = imageSize.x / imageSize.y * size.y;
						else //Prioritize Y
							size.y = imageSize.y / imageSize.x * size.x;
					}
					
					sprite->width = size.x;
					sprite->height = size.y;

					editor()->selectedActor(sprite);
				}
			}
		}
		else if (Mouse::released(Mouse::Left))
		{
			dragging = false;
			draggingCorner = false;
		}
		
		auto* graphic = dynamic_cast<Graphic*>(editor()->selectedActor());
		if (graphic != nullptr)
		{
			Graphic::AABB const aabb = graphic->getAABB();
			Vector2 const screenScale = Vector2(width(), height()) / Vector2(1920, 1080);
			Vector2 const scalar = screenScale * Camera::main()->zoom;
			Vector2 const size = (aabb.max - aabb.min) * scalar;
			Vector2 const cameraPos = (Vector2)Camera::main()->position * scalar;

			Vector2 position = { width() / 2.0f, height() / 2.0f }; //Start at center of the screen coz tiles start there too
			position -= Vector2{ size.x / 2.0f, -size.y / 2.0f }; //Adjust center 
			position += graphic->position * scalar; //Adjust based on tile index

			highlight->move(position.x - cameraPos.x, height() - position.y + cameraPos.y);
			highlight->resize(size.x, size.y);
			highlight->show();

			corner->move(highlight->pos() + highlight->rect().topRight() - QPoint(size.x / 10.0f, size.y / 10.0f));
			corner->resize(size.x / 5.0f, size.y / 5.0f);
			corner->show();
		}
	}

	void ActorLayerSceneView::clickActor()
	{
		Vector2 const world = GameView::screenToWorld(Mouse::position());
		for (size_t i = 0; i < actorLayer->getActorCount(); i++)
		{
			auto* graphic = dynamic_cast<Graphic*>(actorLayer->getActor(i));

			if (graphic != nullptr)
			{
				if (graphic->withinBounds(world))
				{
					if (graphic == editor()->selectedActor())
						dragging = true;

					editor()->selectedActor(graphic);
					return;
				}
			}
		}

		dragging = false;
		draggingCorner = false;
		highlight->hide();
		corner->hide();
		editor()->selectedActor(nullptr);
	}
}

#endif