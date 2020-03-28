#pragma once
#include <Physics/Collider.h>

namespace Tristeon
{
	class BoxCollider : public Collider
	{
		REGISTER_TYPE_H(BoxCollider)
	public:
		json serialize() override;
		void deserialize(json j) override;

		float width() const;
		void width(float const& value);

		float height() const;
		void height(float const& value);

	protected:
		float _width = 0;
		float _height = 0;
		
		void createShape() override;
	};
}
