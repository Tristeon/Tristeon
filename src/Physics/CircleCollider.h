#pragma once
#include <Physics/Collider.h>
#include <Registers/BehaviourRegister.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * CircleCollider is a Collider that defines its shape as a circle with a single radius.
	 */
	class CircleCollider : public Collider
	{
		REGISTER_BEHAVIOUR_H(CircleCollider)
		REGISTER_TYPE_H(CircleCollider)
	public:
		virtual ~CircleCollider() = default;
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets the radius of the circle in pixels.
		 */
		float radius() const;
		/**
		 * Sets the radius of the circle.
		 */
		void radius(float const& value);

	protected:
		float _radius = 0;

		void createShape(bool const& includeBodyTransform) override;
	};
}