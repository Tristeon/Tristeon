#ifdef TRISTEON_EDITOR
#include "Input/Keyboard.h"
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

		outline = new QLabel(this);
		outline->setPixmap(QPixmap("Internal/Textures/outline.png"));
		outline->setScaledContents(true);
		outline->setAttribute(Qt::WA_TranslucentBackground);
		outline->hide();

		corner = new QLabel(this);
		corner->setPixmap(QPixmap("Internal/Textures/selection.png"));
		corner->setScaledContents(true);
		corner->hide();

		rotate = new QLabel(this);
		rotate->setPixmap(QPixmap("Internal/Textures/rotate.png"));
		rotate->setScaledContents(true);
		rotate->setAttribute(Qt::WA_TranslucentBackground);
		rotate->hide();
	}

	void ActorLayerSceneView::updateView()
	{
		if (Engine::playMode())
		{
			outline->hide();
			corner->hide();
			rotate->hide();
			
			dragging = false;
			draggingCorner = false;
			draggingRotate = false;
			return;
		}

		if (Mouse::pressed(Mouse::Left) && GameView::instance()->widget()->underMouse())
		{
			if ((corner->isHidden() || !corner->underMouse()) && (rotate->isHidden() || !rotate->underMouse()))
				clickActor();
			else if (!corner->isHidden() && corner->underMouse())
			{
				draggingCorner = true;
			}
			else if (!rotate->isHidden() && rotate->underMouse())
			{
				draggingRotate = true;
			}
		}
		else if (Mouse::held(Mouse::Left) && Editor::instance()->selectedActor() != nullptr)
		{
			if (dragging)
			{
				Editor::instance()->selectedActor()->position = GameView::screenToWorld(Mouse::position());
			}
			else if (draggingCorner)
			{
				auto* sprite = dynamic_cast<Sprite*>(Editor::instance()->selectedActor());

				if (sprite != nullptr && sprite->getTexture() != nullptr)
				{
					Vector2 const position = Editor::instance()->selectedActor()->position;
					Vector2 const topRight = GameView::screenToWorld(Mouse::position());
					Vector2 const difference = topRight - position;
					if (difference.x < 0 || difference.y < 0)
						return;

					Vector2 size = difference / sprite->scale * 2;

					//Snap to aspect ratio
					if (Keyboard::held(Keyboard::Shift))
					{
						Vector2 const imageSize = sprite->getTexture()->size();
						
						if (size.x > size.y) //Prioritize X
							size.x = imageSize.x / imageSize.y * size.y;
						else //Prioritize Y
							size.y = imageSize.y / imageSize.x * size.x;
					}
					
					sprite->width = size.x;
					sprite->height = size.y;
				}
			}
			else if (draggingRotate)
			{
				Vector2 const position = Editor::instance()->selectedActor()->position;
				Vector2 const mouse = (GameView::screenToWorld(Mouse::position()) - position).getNormalized();
				
				Editor::instance()->selectedActor()->rotation = mouse.getAngle();
			}
		}
		else if (Mouse::released(Mouse::Left))
		{
			dragging = false;
			draggingCorner = false;
			draggingRotate = false;
			Editor::instance()->selectedActor(Editor::instance()->selectedActor());
		}
		
		auto* graphic = dynamic_cast<Graphic*>(Editor::instance()->selectedActor());
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

			outline->move(position.x - cameraPos.x, height() - position.y + cameraPos.y);
			outline->resize(size.x, size.y);
			outline->show();
			
			corner->move(outline->pos() + outline->rect().topRight() - QPoint(size.x / 10.0f, size.y / 10.0f));
			corner->resize(size.x / 5.0f, size.y / 5.0f);
			corner->show();

			Vector2 const unit = Vector2::unit(graphic->rotation);
			rotate->move(outline->pos() + outline->rect().center() - QPoint(size.x / 10.0f, size.y / 10.0f) + QPoint(unit.x * outline->rect().width(), unit.y * outline->rect().height()));
			rotate->resize(size.x / 5.0f, size.y / 5.0f);
			rotate->show();
		}
		else
		{
			outline->hide();
			corner->hide();
			rotate->hide();

			dragging = false;
			draggingCorner = false;
			draggingRotate = false;
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
					if (graphic == Editor::instance()->selectedActor())
						dragging = true;

					Editor::instance()->selectedActor(graphic);
					return;
				}
			}
		}
	}
}
#endif