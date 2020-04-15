#pragma once
#include <Physics/Collider.h>

namespace Tristeon
{
	/**
	 * BoxCollider is a collider that defines its shape as a box or 2D rectangle.
	 *
	 * The box is defined using a width and height for x min-max and y min-max respectively.
	 */
	class BoxCollider : public Collider
	{
		REGISTER_TYPE_H(BoxCollider)
	public:
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets the width of the box in pixels.
		 */
		float width() const;
		/**
		 * Sets the width of the box.
		 */
		void width(float const& value);

		/**
		 * Gets the height of the box in pixels.
		 */
		float height() const;
		/**
		 * Sets the height of the box.
		 */
		void height(float const& value);
		
	protected:
		float _width = 0;
		float _height = 0;
		
		void createShape(bool const& includeBodyTransform) override;
	};
}