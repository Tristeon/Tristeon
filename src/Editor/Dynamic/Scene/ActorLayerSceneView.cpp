#ifdef TRISTEON_EDITOR
#include "ActorLayerSceneView.h"
#include <Input/Mouse.h>
#include "Engine.h"
#include "Window.h"
#include "Editor/Editor.h"
#include "Editor/GameView.h"
#include "Input/Keyboard.h"
#include "Math/Math.h"
#include "Rendering/Camera.h"
#include "Rendering/Gizmos.h"
#include "Rendering/Graphic.h"
#include "Rendering/Grid.h"
#include "Rendering/Sprite.h"

using namespace Tristeon;
namespace TristeonEditor
{
	SCENE_EDITOR_CPP(Tristeon::ActorLayer, ActorLayerSceneView);
	
	void ActorLayerSceneView::initialize()
	{
		actorLayer = dynamic_cast<ActorLayer*>(target());
	}

	void ActorLayerSceneView::updateView()
	{
		if (Engine::playMode())
		{
			dragging = false;
			draggingScalar = false;
			draggingRotate = false;
			return;
		}

		if (Mouse::pressed(Mouse::Left) && GameView::instance()->underMouse())
		{
			if (scalar.underMouse())
				draggingScalar = true;
			else if (rotator.underMouse())
				draggingRotate = true;
			else
				clickActor();
		}
		else if (Mouse::held(Mouse::Left) && Editor::instance()->selectedActor() != nullptr && !Keyboard::held(Keyboard::Alt))
		{
			if (dragging)
			{
				if (Keyboard::held(Keyboard::Shift))
				{
					auto* graphic = dynamic_cast<Graphic*>(Editor::instance()->selectedActor());
					if (graphic == nullptr)
						Editor::instance()->selectedActor()->position = Grid::snap(Window::screenToWorld(Mouse::position(), Renderer::editorCamera()) - draggingOffset);
					else
						graphic->position = Grid::snap(Window::screenToWorld(Mouse::position(), Renderer::editorCamera()) - draggingOffset) + graphic->bounds().size() / 2.0f - Grid::tileSize() / 2.0f;
				}
				else
					Editor::instance()->selectedActor()->position = Window::screenToWorld(Mouse::position(), Renderer::editorCamera()) - draggingOffset;
			}
			else if (draggingScalar)
			{
				//TODO: Make graphic resizing more generic
				auto* sprite = dynamic_cast<Sprite*>(Editor::instance()->selectedActor());

				if (sprite != nullptr && sprite->texture() != nullptr)
				{
					Vector const position = Editor::instance()->selectedActor()->position;
					Vector const topRight = Window::screenToWorld(Mouse::position(), Renderer::editorCamera());
					Vector const difference = topRight - position;
					if (difference.x < 0 || difference.y < 0)
						return;

					Vector size = difference / sprite->scale * 2;

					//Snap to aspect ratio
					if (Keyboard::held(Keyboard::Shift))
					{
						VectorI const imageSize = sprite->texture()->size();
						
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
				Vector const position = Editor::instance()->selectedActor()->position;
				Vector const mouse = (Window::screenToWorld(Mouse::position(), Renderer::editorCamera()) - position).normalized();
				const float angle = mouse.angle();

				if (Keyboard::held(Keyboard::Shift))
					Editor::instance()->selectedActor()->rotation = roundf(angle / 45.0f) * 45.0f;
				else
					Editor::instance()->selectedActor()->rotation = angle;
			}
		}
		else if (Mouse::released(Mouse::Left))
		{
			dragging = false;
			draggingScalar = false;
			draggingRotate = false;
			Editor::instance()->selectedActor(Editor::instance()->selectedActor());
		}
		
		auto* graphic = dynamic_cast<Graphic*>(Editor::instance()->selectedActor());
		if (graphic != nullptr)
		{
			Graphic::Bounds const aabb = graphic->bounds();
			Gizmos::drawSquare(graphic->position, aabb.size(), graphic->rotation, Colour(0.5, 0.5, 1, 1));

			const Vector handleSize = Vector(Math::clamp(32 / Renderer::editorCamera()->zoom, 8.0f, 256.0f), Math::clamp(32 / Renderer::editorCamera()->zoom, 8.0f, 256.0f));
			const Vector cpos = Math::orbit(graphic->position, aabb.size() / 2.0f, graphic->rotation);
			Gizmos::drawSquare(cpos, handleSize, graphic->rotation, Colour(0.5, 0.5, 0.5));
			scalar = Graphic::Bounds{ cpos - handleSize / 2.0f, cpos + handleSize / 2.0f };

			const Vector rpos = Math::orbit(graphic->position, Vector(aabb.size().x, 0), graphic->rotation);
			Gizmos::drawSquare(rpos, handleSize, graphic->rotation, Colour(0, 0.5, 0.5));
			rotator = Graphic::Bounds{ rpos - handleSize / 2.0f, rpos + handleSize / 2.0f };
		}
		else
		{
			dragging = false;
			draggingScalar = false;
			draggingRotate = false;
		}
	}

	void ActorLayerSceneView::clickActor()
	{
		Vector const world = Window::screenToWorld(Mouse::position(), Renderer::editorCamera());
		for (size_t i = 0; i < actorLayer->actorCount(); i++)
		{
			auto* graphic = dynamic_cast<Graphic*>(actorLayer->actorAt(i));

			if (graphic != nullptr)
			{
				if (graphic->bounds().contains(world))
				{
					if (graphic == Editor::instance()->selectedActor())
					{
						dragging = true;
						draggingOffset = world - graphic->position;
					}

					Editor::instance()->selectedActor(graphic);
					return;
				}
			}
		}

		Editor::instance()->selectedActor(nullptr);
		dragging = false;
		draggingScalar = false;
		draggingRotate = false;
	}
}
#endif