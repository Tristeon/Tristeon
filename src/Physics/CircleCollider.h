#pragma once
#include <Physics/Collider.h>

namespace Tristeon
{
	class CircleCollider : public Collider
	{
		REGISTER_TYPE_H(CircleCollider)
	public:
		json serialize() override;
		void deserialize(json j) override;

		float radius() const;
		void radius(float const& value);

	protected:
		float _radius = 0;

		void createShape() override;
	};
}
