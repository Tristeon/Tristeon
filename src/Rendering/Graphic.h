#pragma once
#include "Actors/Actor.h"
#include <Rendering/Shader.h>

namespace Tristeon
{
	class ActorLayer;

	/**
	 * Graphic is the base class for 2D renderable actors.
	 *
	 * Inheriting classes must override the render(), bounds(), and getShader() functions.
	 */
	class Graphic : public Actor
	{
		friend ActorLayer;
	public:
		Graphic() = default;
		virtual ~Graphic() = default;

		DELETE_COPY(Graphic);
		DEFAULT_MOVE(Graphic);

		json serialize() override;
		void deserialize(json j) override;
		
		/**
		 * A square, defined by a min and max value.
		 * Bounds are used to describe the smallest axis aligned square that can fit around a Graphic.
		 */
		struct Bounds
		{
			Vector2 min;
			Vector2 max;

			/**
			 * Returns true if the position is between min and max.
			 */
			[[nodiscard]] bool contains(Vector2 const& position) const;
			/**
			 * Calculates the size by doing (max - min).
			 */
			[[nodiscard]] Vector2 size() const;
			/**
			 * Returns true if the mouse is hovering over the bounds.
			 */
			[[nodiscard]] bool underMouse() const;
		};
		
		/**
		 * Returns the Graphic's Bounds.
		 * The Bounds is a square defined by a min and max value, this does not have to accurately reflect the shape of the object (e.g. the bounds of a circle are still simply a square)
		 */
		virtual Bounds bounds() = 0;

		/**
		 * Enables or disables rendering the graphic.
		 */
		bool display = true;
		
	protected:
		/**
		 * Render the graphic to the GameView, called for each graphic by the ActorLayer.
		 *
		 * Inheriting classes should override this function to implement rendering behaviour.
		 */
		virtual void render() = 0;

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
}