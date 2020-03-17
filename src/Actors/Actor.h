#pragma once
#include <Actors/Behaviours/Behaviour.h>
#include <Math/Vector2Int.h>
#include <TypeDefinitions.h>

#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	class ActorLayer;
	class SceneManager;

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
	class Actor : public Serializable
	{
		REGISTER_TYPE_H(Actor)
		
		friend ActorLayer;
		friend SceneManager;
	public:
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
		std::string name = "";
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Returns the actor's tag.
		 * Tags are an additional way to identify actors. Unlike names, tags are chosen from a list of pre-defined tags to simplify checking.
		 */
		std::string getTag() const { return tag; }

		/**
		 * Gets the first behaviour of type T. nullptr if no behaviour of type T was found.
		 */
		template<typename T>
		IsBehaviour<T>* behaviour();

		/**
		 * Adds a new behaviour of type T and returns the new behaviour.
		 */
		template<typename T>
		IsBehaviour<T>* addBehaviour();

		/**
		 * The update function, to be overridden by derived classes.
		 */
		virtual void update() { /* Empty */ }
	private:
		Vector<Unique<Behaviour>> behaviours;
		std::string tag = "";
	};

	template <typename T>
	IsBehaviour<T>* Actor::behaviour()
	{
		for (auto const& behaviour : behaviours)
		{
			T* result = dynamic_cast<T*>(behaviour.get());
			if (result != nullptr)
				return result;
		}
		return nullptr;
	}

	template <typename T>
	IsBehaviour<T>* Actor::addBehaviour()
	{
		T* result = new T();
		result->_owner = this;
		behaviours.push_back(Unique<T>(result));
		return result;
	}
}
