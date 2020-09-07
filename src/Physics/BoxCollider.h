#pragma once
#include <Physics/Collider.h>
#include <Serialization/TypeRegister.h>
#include "Callbacks/IDrawGizmos.h"

namespace Tristeon
{
	/**
	 * BoxCollider is a collider that defines its shape as a box or 2D rectangle.
	 * The box is defined using a width and height for x min-max and y min-max respectively.
	 *
	 * BoxCollider uses the Box2D API and as such all coordinates and sizes are internally converted to meters.
	 */
	class BoxCollider : public Collider, public IDrawGizmos
	{
	public:
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets the width of the box in pixels.
		 */
		[[nodiscard]] float width() const;
		/**
		 * Sets the width of the box.
		 */
		void setWidth(const float& value);

		/**
		 * Gets the height of the box in pixels.
		 */
		[[nodiscard]] float height() const;
		/**
		 * Sets the height of the box.
		 */
		void setHeight(const float& value);
		
	protected:
		float _width = 1;
		float _height = 1;
		
		void drawGizmos() override;
		void createShape(const bool& includeBodyTransform) override;
	};

	REGISTER_TYPE(BoxCollider);
}