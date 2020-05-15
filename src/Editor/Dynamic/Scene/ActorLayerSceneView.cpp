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
	}

	void ActorLayerSceneView::updateView()
	{
		if (Engine::playMode())
		{
			highlight->hide();
			return;
		}

		if (Mouse::released(Mouse::Left))
		{
			Vector2 const world = GameView::screenToWorld(Mouse::position());
			for (size_t i = 0; i < actorLayer->getActorCount(); i++)
			{
				auto* graphic = dynamic_cast<Graphic*>(actorLayer->getActor(i));
				if (graphic != nullptr && graphic != editor()->selectedActor())
				{
					if (graphic->withinBounds(world))
					{
						editor()->selectedActor(graphic);
						return;
					}
				}
			}

			highlight->hide();
			editor()->selectedActor(nullptr);
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
		}
	}
}

#endif