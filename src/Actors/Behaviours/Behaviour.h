#pragma once
#include <Serialization/Serializable.h>
#include <Utils/Vector.h>

namespace Tristeon
{
	class Actor;
	class ActorLayer;

	/**
	 * A Behaviour in Tristeon is a class that can be added to an Actor to dynamically add additional behaviour to the Actor object.
	 *
	 * Behaviours are useful because they can be reused and added to different objects,
	 * enabling programmers to generalize behaviour, reduce code and simplify the inheritance tree.
	 */
	class Behaviour : public Serializable
	{
		friend Actor;
		friend ActorLayer;
	public:
		Behaviour();
		virtual ~Behaviour();
		Behaviour(const Behaviour& other) = delete;
		Behaviour& operator=(const Behaviour& other) = delete;

		json serialize() override = 0;
		void deserialize(json j) override = 0;

		Behaviour(Behaviour&& other) noexcept;
		Behaviour& operator=(Behaviour&& other) noexcept;

		/**
		 * Behaviours are owned by an actor. This value is never nullptr.
		 */
		Actor* owner() const { return _owner; }
	private:
		Actor* _owner = nullptr;
	};
}
