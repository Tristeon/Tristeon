#pragma once
#include <Actors/Behaviours/Behaviour.h>
#include <Serialization/TypeRegister.h>
#include <Math/Vector2.h>

#include <box2d/b2_shape.h>
#include <box2d/b2_fixture.h>

#include <Callbacks/IStart.h>
#include <Callbacks/ILateUpdate.h>

namespace Tristeon
{
	class Collider : public Behaviour, public IStart, public ILateUpdate
	{
		friend class PhysicsBody;
		REGISTER_TYPE_H(Collider);
	public:
		virtual ~Collider();
		
		void start() override;
		void lateUpdate() override;
		
		json serialize() override;
		void deserialize(json j) override;

		float width() const;
		void width(float const& value);

		float height() const;
		void height(float const& value);

		float density() const;
		void density(float const& value);

		float friction() const;
		void friction(float const& value);

		float restitution() const;
		void restitution(float const& value);

		Vector2 offset() const;
		void offset(Vector2 const& value);

		float rotationOffset() const;
		void rotationOffset(float const& value);

		bool sensor() const;
		void sensor(bool const& value);

	protected:
		//Shape definition
		float _width = 0;
		float _height = 0;
		Vector2 _offset = {};
		float _rotationOffset = 0;

		//Simulation values
		float _density = 0;
		float _friction = 0;
		float _restitution = 0;

		bool _sensor = false;
		
		std::unique_ptr<b2Shape> shape;
		virtual void createShape();

		b2Fixture* getFixture();
		b2Shape* getShape();

		bool isDirty = false;
	};
}
