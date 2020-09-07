#pragma once
#include <Physics/Collider.h>
#include <Serialization/TypeRegister.h>
#include "Callbacks/IDrawGizmos.h"

namespace Tristeon
{
	/**
	 * CircleCollider is a Collider that defines its shape as a circle with a single radius.
	 *
	 * CircleCollider uses the Box2D API and as such all coordinates and sizes are internally converted to meters.
	 */
	class CircleCollider : public Collider, public IDrawGizmos
	{
	public:
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets the radius of the circle in pixels.
		 */
		[[nodiscard]] float radius() const;
		/**
		 * Sets the radius of the circle.
		 */
		void setRadius(const float& value);

	protected:
		float _radius = 0;

		void drawGizmos() override;
		void createShape(const bool& includeBodyTransform) override;
	};

	REGISTER_TYPE(CircleCollider);
}