#include "Input/Keyboard.h"
#include "ActorLayerSceneView.h"
#include <Input/Mouse.h>
#include "Engine.h"
#include "Window.h"
#include "Editor/Editor.h"
#include "Editor/GameView.h"
#include "Math/Math.h"
#include "Rendering/Gizmos.h"
#include "Rendering/Graphic.h"
#include "Rendering/Grid.h"
#include "Rendering/Sprite.h"

namespace TristeonEditor
{
	void ActorLayerSceneView::updateView()
	{
		if (Tristeon::Engine::playMode())
		{
			_dragging = false;
			_draggingScalar = false;
			_draggingRotate = false;
			return;
		}

		if (Tristeon::Mouse::pressed(Tristeon::Mouse::Left) && GameView::instance()->underMouse())
		{
			if (_scalar.underMouse())
				_draggingScalar = true;
			else if (_rotator.underMouse())
				_draggingRotate = true;
			else
				clickActor();
		}
		else if (Tristeon::Mouse::held(Tristeon::Mouse::Left) && Editor::instance()->selectedActor() != nullptr && !Tristeon::Keyboard::held(Tristeon::Keyboard::Alt))
		{
			if (_dragging)
			{
				if (Tristeon::Keyboard::held(Tristeon::Keyboard::Shift))
				{
					auto* graphic = dynamic_cast<Tristeon::Graphic*>(Editor::instance()->selectedActor());
					if (graphic == nullptr)
						Editor::instance()->selectedActor()->position = Tristeon::Grid::snap(Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera()) - _draggingOffset);
					else
						graphic->position = Tristeon::Grid::snap(Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera()) - _draggingOffset) + graphic->bounds().size() / 2.0f - Tristeon::Grid::tileSize() / 2.0f;
				}
				else
					Editor::instance()->selectedActor()->position = Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera()) - _draggingOffset;
			}
			else if (_draggingScalar)
			{
				//TODO: Make graphic resizing more generic
				auto* sprite = dynamic_cast<Tristeon::Sprite*>(Editor::instance()->selectedActor());

				if (sprite != nullptr && sprite->texture() != nullptr)
				{
					Tristeon::Vector const position = Editor::instance()->selectedActor()->position;
					Tristeon::Vector const topRight = Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera());
					Tristeon::Vector const difference = topRight - position;
					if (difference.x < 0 || difference.y < 0)
						return;

					Tristeon::Vector size = difference / sprite->scale * 2;

					//Snap to aspect ratio
					if (Tristeon::Keyboard::held(Tristeon::Keyboard::Shift))
					{
						Tristeon::VectorI const imageSize = sprite->texture()->size();
						
						if (size.x > size.y) //Prioritize X
							size.x = imageSize.x / imageSize.y * size.y;
						else //Prioritize Y
							size.y = imageSize.y / imageSize.x * size.x;
					}
					
					sprite->size = size;
				}
			}
			else if (_draggingRotate)
			{
				Tristeon::Vector const position = Editor::instance()->selectedActor()->position;
				Tristeon::Vector const mouse = (Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera()) - position).normalize();
				const float angle = mouse.angle();

				if (Tristeon::Keyboard::held(Tristeon::Keyboard::Shift))
					Editor::instance()->selectedActor()->rotation = roundf(angle / 45.0f) * 45.0f;
				else
					Editor::instance()->selectedActor()->rotation = angle;
			}
		}
		else if (Tristeon::Mouse::released(Tristeon::Mouse::Left))
		{
			_dragging = false;
			_draggingScalar = false;
			_draggingRotate = false;
		}
		
		auto* graphic = dynamic_cast<Tristeon::Graphic*>(Editor::instance()->selectedActor());
		if (graphic != nullptr)
		{
			Tristeon::Graphic::Bounds const aabb = graphic->bounds();
			Tristeon::Gizmos::drawSquare(graphic->position, aabb.size(), graphic->rotation, Tristeon::Colour(0.5, 0.5, 1, 1));

			const Tristeon::Vector handleSize = Tristeon::Vector(Tristeon::Math::clamp(32 / Tristeon::Renderer::editorCamera()->zoom, 8.0f, 256.0f), Tristeon::Math::clamp(32 / Tristeon::Renderer::editorCamera()->zoom, 8.0f, 256.0f));
			const Tristeon::Vector cpos = Tristeon::Math::orbit(graphic->position, aabb.size() / 2.0f, graphic->rotation);
			Tristeon::Gizmos::drawSquare(cpos, handleSize, graphic->rotation, Tristeon::Colour(0.5, 0.5, 0.5));
			_scalar = Tristeon::Graphic::Bounds{ cpos - handleSize / 2.0f, cpos + handleSize / 2.0f };

			const Tristeon::Vector rpos = Tristeon::Math::orbit(graphic->position, Tristeon::Vector(aabb.size().x, 0), graphic->rotation);
			Tristeon::Gizmos::drawSquare(rpos, handleSize, graphic->rotation, Tristeon::Colour(0, 0.5, 0.5));
			_rotator = Tristeon::Graphic::Bounds{ rpos - handleSize / 2.0f, rpos + handleSize / 2.0f };
		}
		else
		{
			_dragging = false;
			_draggingScalar = false;
			_draggingRotate = false;
		}
	}

	void ActorLayerSceneView::clickActor()
	{
		Tristeon::Vector const world = Tristeon::Window::screenToWorld(Tristeon::Mouse::position(), Tristeon::Renderer::editorCamera());
		for (size_t i = 0; i < _actorLayer->actorCount(); i++)
		{
			auto* graphic = dynamic_cast<Tristeon::Graphic*>(_actorLayer->actorAt(i));

			if (graphic != nullptr)
			{
				if (graphic->bounds().contains(world))
				{
					if (graphic == Editor::instance()->selectedActor())
					{
						_dragging = true;
						_draggingOffset = world - graphic->position;
					}

					Editor::instance()->selectedActor(graphic);
					return;
				}
			}
		}

		Editor::instance()->selectedActor(nullptr);
		_dragging = false;
		_draggingScalar = false;
		_draggingRotate = false;
	}
}