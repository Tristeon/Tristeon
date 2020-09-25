#pragma once
#include <Serialization/InstancedSerializable.h>

namespace Tristeon
{
	/**
	 * A Behaviour in Tristeon is a class that can be added to an Actor to dynamically add additional behaviour to the Actor object.
	 *
	 * Behaviours are useful because they can be reused and added to different objects,
	 * enabling programmers to generalize behaviour, reduce code and simplify the inheritance tree.
	 */
	class Behaviour : public InstancedSerializable
	{
		friend class Actor;
		friend class ActorLayer;
	public:
		Behaviour();
		virtual ~Behaviour();

		DELETE_COPY(Behaviour);

		Behaviour(Behaviour&& other) noexcept;
		Behaviour& operator=(Behaviour&& other) noexcept;

		/**
		 * Queues the Behaviour up for destruction, and then at the right time safely removes the behaviour from its actor and deletes the Behaviour instance.
		 * destroy() should be used at all times as opposed to manual deletion, to avoid deleting objects within critical loops.
		 */
		void destroy();

		/**
		 * Gets the actor that the behaviour is attached to.
		 * The template parameter can be used to conveniently cast the actor to a specific type. This performs a simple dynamic_cast and will return nullptr upon failure.
		 */
		template <typename T = Actor>
		T* actor() const;

	private:
		bool _destroyed = false;
		Actor* _actor = nullptr;
	};

	template <typename T>
	T* Behaviour::actor() const
	{
		return dynamic_cast<T*>(_actor);
	}

	template <>
	inline Actor* Behaviour::actor() const
	{
		return _actor;
	}
}