#pragma once
#include "Serialization/InstancedSerializable.h"
#include "Behaviour.h"

#include "Math/Vector2.h"

#include <Callbacks/IStart.h>
#include <Collector.h>

namespace Tristeon
{
	template<typename T>
	using IsBehaviour = std::enable_if_t<std::is_base_of<Behaviour, T>::value, T>;

	/**
	 * An actor is an empty container that can exist freely on an ActorLayer.
	 * Actors can be inherited from to implement more specific actors such as Player, Car, Enemy, etc.
	 * In addition to that, you can add Behaviours to actors, providing extra flexibility and re usability.
	 *
	 * Within Tristeon, we recommend to make the core object-defining classes such as Enemy or Gun derived classes of Actor,
	 * while additional and potentially reusable behaviours such as a FollowBehaviour should be added as Behaviours.
	 * 
	 * Unlike Tiles, Actors can move, scale and rotate around the level freely, unbound by the grid or by axis alignment.
	 */
	class Actor : public InstancedSerializable
	{
		friend class ActorLayer;
		friend class SceneManager;
		friend class Engine;
	public:
		Actor();
		explicit Actor(const bool& registerSelf);
		virtual ~Actor() noexcept;

		DELETE_COPY(Actor);
		DEFAULT_MOVE(Actor);

		json serialize() override;
		void deserialize(json j) override;
		
		/**
		 * The current position in 2D space of the actor, in pixels.
		 */
		Vector2 position = { 0, 0 };

		/**
		 * The current scale of the actor, Vector2::one() by default.
		 */
		Vector2 scale = { 1, 1 };

		/**
		 * The current rotation of the actor, in degrees.
		 */
		float rotation = 0;

		/**
		 * The name of the actor, used to identify the actor.
		 */
		String name = "";

		/**
		 * Gets the first behaviour of type T. nullptr if no behaviour of type T was found.
		 */
		template<typename T>
		IsBehaviour<T>* behaviour();

		/**
		 * Gets a list of all the behaviours of type T. Returns an empty vector if no behaviour of type T was found.
		 */
		template<typename T>
		Vector<T*> behaviours();

		/**
		 * Gets a list with all of the actor's behaviours.
		 */
		Vector<Behaviour*> behaviours();
		
		/**
		 * Adds a new behaviour of type T and returns the new behaviour.
		 * If the type isn't found in the register then this function returns nullptr.
		 */
		template<typename T>
		IsBehaviour<T>* addBehaviour();

		/**
		 * Adds a new behaviour of the given type and returns said behaviour.
		 * If the type is not recognized as a behaviour, or if the type isn't found in the register at all then this function returns nullptr.
		 */
		Behaviour* addBehaviour(const String& type);

		/**
		 * Destroy removes the actor from the scene and deletes the Actor's instance.
		 * destroy() should be used at all times as opposed to manual deletion, to avoid deleting objects within critical loops.
		 */
		void destroy();

		/**
		 * Looks through every actor layer,
		 * returns the first actor with the given name.
		 *
		 * Returns nullptr if no actor was found.
		 */
		static Actor* find(const String& name);

		/**
		 * Returns the actor with the given ID.
		 * Returns nullptr if no such actor exists.
		 */
		static Actor* find(const unsigned int& id);

		/**
		 * Returns the first actor with the given type.
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		static T* findOfType();

		/**
		 * Returns the first actor with the given type AND name.
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		static T* findOfType(const String& name);
	private:
		Vector<Unique<Behaviour>> _behaviours{};
		bool _destroyed = false;

		/**
		 * Removes and destroys the given behaviour.
		 * Used internally by Engine to destroy behaviours outside of critical loops.
		 */
		void removeBehaviour(Behaviour* behaviour);
	};

	template <typename T>
	IsBehaviour<T>* Actor::behaviour()
	{
		for (auto const& behaviour : _behaviours)
		{
			T* result = dynamic_cast<T*>(behaviour.get());
			if (result != nullptr)
				return result;
		}
		return nullptr;
	}

	template <typename T>
	Vector<T*> Actor::behaviours()
	{
		Vector<T*> behaviours;

		for (auto const& behaviour : _behaviours)
		{
			T* value = dynamic_cast<T*>(behaviour.get());
			if (value != nullptr)
				behaviours.add(value);
		}

		return behaviours;
	}

	template <typename T>
	IsBehaviour<T>* Actor::addBehaviour()
	{
		T* result = new T();
		result->_actor = this;
		_behaviours.add(Unique<Behaviour>(result));

		//Call start callback if available.
		auto* istart = dynamic_cast<IStart*>(result);
		if (istart != nullptr)
			istart->start();
		
		return result;
	}

	template <typename T>
	T* Actor::findOfType()
	{
		auto actors = Collector<Actor>::all();
		for (auto actor : actors)
		{
			if (dynamic_cast<T*>(actor))
				return dynamic_cast<T*>(actor);
		}
		return nullptr;
	}

	template <typename T>
	T* Actor::findOfType(const String& name)
	{
		auto actors = Collector<Actor>::all();
		for (auto actor : actors)
		{
			if (actor->name == name && dynamic_cast<T*>(actor))
				return dynamic_cast<T*>(actor);
		}
		return nullptr;
	}
}