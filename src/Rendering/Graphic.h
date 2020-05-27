#pragma once
#include "Actors/Actor.h"
#include <Rendering/Shader.h>

namespace Tristeon
{
	class ActorLayer;

	/**
	 * Graphic is the base class for 2D renderable actors.
	 *
	 * Inheriting classes must override the render() and getShader() functions.
	 */
	class Graphic : public Actor
	{
		friend ActorLayer;
	public:
		struct AABB
		{
			Vector2 min;
			Vector2 max;

			bool contains(Vector2 const& position) const
			{
				return position.x > min.x&& position.x < max.x&& position.y > min.y&& position.y < max.y;
			}
		};
		
		/**
		 * Returns true if the world position is within the bounds of the Graphic.
		 * This function is overridden in inherited classes to accurately represent its bounds
		 */
		virtual bool withinBounds(Vector2 const& worldPos) = 0;

		/**
		 * Returns the Graphic's AABB.
		 * The AABB is a square defined by a min and max value, this does not have to accurately reflect the shape of the object (e.g. the bounds of a circle are still simply a square)
		 */
		virtual AABB getAABB() = 0;

		bool display = true;

		json serialize() override;
		void deserialize(json j) override;
	protected:
		/**
		 * Render the graphic to the GameView, called for each graphic by the ActorLayer.
		 *
		 * Inheriting classes should override this function to implement rendering behaviour.
		 */
		virtual void render(QOpenGLShaderProgram* program) = 0;

		/**
		 * Get a pointer to the Graphic's shader.
		 * Inheriting classes are recommended to own a static version of their own shader,
		 * initialized within this function as follows:
		 *
		 * Shader* InheritingClass::getShader() {
		 *		static Shader shader = Shader("VertexPath", "FragmentPath");
		 *
		 *		return &shader;
		 * }
		 *
		 * This way, the implementation cost is minimal and the result is performant as the shader is only created once.
		 */
		virtual Shader* getShader() = 0;
	};

	inline json Graphic::serialize()
	{
		json j = Actor::serialize();
		j["display"] = display;
		return j;
	}

	inline void Graphic::deserialize(json j)
	{
		Actor::deserialize(j);
		display = j.value("display", true);
	}
}