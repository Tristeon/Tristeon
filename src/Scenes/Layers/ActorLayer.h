#pragma once
#include "Layer.h"
#include <Serialization/TypeRegister.h>
#include "Scenes/Actors/Actor.h"

namespace Tristeon
{
	class Shader;

	/**
	 * The actor layer contains a list of actors.
	 *
	 * Actors are objects which are free to move, rotate and scale around, unlimited by the tile-grid.
	 * The layer updates & renders them, and acts as a way for other classes to freely interact with actors.
	 */
	class ActorLayer : public Layer
	{
		friend class Scene;
		friend class SceneManager;

		friend class Actor;
		friend class Behaviour;
	public:
		ActorLayer() = default;
		virtual ~ActorLayer();

		DELETE_COPY(ActorLayer);
		DEFAULT_MOVE(ActorLayer);
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets an actor using the given index.
		 *
		 * \exception invalid_argument Throws if the index is more than actorCount()
		 */
		[[nodiscard]] Actor* actorAt(const unsigned long long& index) const;

		/**
		 * Returns the amount of actors in this layer
		 */
		[[nodiscard]] unsigned long long actorCount() const;

		/**
		 * Returns true if the given actor is in the layer's internal list.
		 */
		[[nodiscard]] bool contains(Actor* actor) const;
		
		/**
		 * Finds the first actor with the given name within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		[[nodiscard]] Actor* findActor(String const& name) const;

		/**
		 * Finds the first actor of the given type within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		[[nodiscard]] T* findActorOfType() const;

		/**
		 * Finds the first actor of the given type and name within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		[[nodiscard]] T* findActorOfType(const String& name) const;

		/**
		 * Returns a list of all the actors of the given type within this layer.
		 *
		 * Returns an empty list if no actors were found.
		 *
		 * Compilation fails if T does not derive from Actor.
		 */
		template<typename T>
		[[nodiscard]] List<T*> findActorsOfType() const;

		/**
		 * Creates and returns a new actor of type T.
		 */
		template<typename T>
		[[nodiscard]] T* createActor();

		/**
		 * Creates and returns a new Actor of the given type.
		 *
		 * Can return nullptr if no such type was registered.
		 */
		[[nodiscard]] Actor* createActor(const String& type);

		/**
		 * Queues the actor up for destruction.
		 * Destruction is processed after each critical loop in the Engine.
		 */
		void destroyActor(Actor* actor);
	protected:

		/**
		 * Queues the behaviour up for destruction.
		 * Destruction is processed after each critical loop in the Engine.
		 */
		void destroyBehaviour(Behaviour* behaviour);
		
		/**
		 * Renders the actors in this layer, in order of the actor list.
		 */
		void render(const Framebuffer& framebuffer) override;

		/**
		 * Cleans up destroyed actors and behaviours.
		 * 
		 * This function is called in-between iteration loops to prevent breaking critical loops.
		 */
		void safeCleanup() override;
		List<Actor*> _destroyedActors{};
		List<Behaviour*> _destroyedBehaviours{};
		
		List<Unique<Actor>> _actors{};
	};

	REGISTER_TYPE(ActorLayer);

	template <typename T>
	T* ActorLayer::findActorOfType() const
	{
		for (auto& actor : _actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				return dynamic_cast<T*>(actor.get());
		}
		return nullptr;
	}

	template <typename T>
	T* ActorLayer::findActorOfType(String const& name) const
	{
		for (auto& actor : _actors)
		{
			if (actor->name == name && dynamic_cast<T*>(actor.get()) != nullptr)
				return dynamic_cast<T*>(actor.get());
		}
		return nullptr;
	}

	template <typename T>
	List<T*> ActorLayer::findActorsOfType() const
	{
		List<T*> result;

		for (auto& actor : _actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				result.add(dynamic_cast<T*>(actor.get()));
		}
		return result;
	}

	template <typename T>
	T* ActorLayer::createActor()
	{
		static_assert(std::is_base_of<Actor, T>::value, "Can't add a new Actor if it isn't of type Actor");
		static_assert(!std::is_abstract<T>::value, "Can't add an abstract Actor!");
		
		T* actor = new T();
		actor->_layer = this;
		_actors.add(std::unique_ptr<T>(actor));
		return actor;
	}
}