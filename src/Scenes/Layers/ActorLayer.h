#pragma once
#include "Layer.h"

#include <Serialization/TypeRegister.h>
#include <Registers/LayerRegister.h>
#include <vector>

#include "Actors/Actor.h"

namespace Tristeon
{
	class Shader;
	class SceneManager;

	/**
	 * The actor layer contains a list of actors.
	 *
	 * Actors are objects which are free to move, rotate and scale around, unlimited by the tile-grid.
	 * The layer updates & renders them, and acts as a way for other classes to freely interact with actors.
	 */
	class ActorLayer : public Layer
	{
		REGISTER_LAYER_H(ActorLayer);
		REGISTER_TYPE_H(ActorLayer);

		friend SceneManager;
	public:
		virtual ~ActorLayer();
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets an actor using the given index.
		 *
		 * \exception invalid_argument Throws if the index is less than 0 or more than getActorCount()
		 */
		Actor* getActor(unsigned int const& index) const;

		/**
		 * Returns the amount of actors in this layer
		 */
		unsigned int getActorCount() const;

		/**
		 * Finds the first actor with the given name within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		Actor* findActor(std::string const& name) const;

		/**
		 * Finds the first actor of the given type within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		T* findActorOfType() const;

		/**
		 * Finds the first actor of the given type and name within this layer.
		 *
		 * Returns nullptr if no actor was found.
		 */
		template<typename T>
		T* findActorOfType(String const& name) const;

		/**
		 * Returns a vector of all the actors of the given type within this layer.
		 *
		 * Returns an empty vector if no actors were found.
		 *
		 * Compilation fails if T does not derive from Actor.
		 */
		template<typename T>
		Vector<T*> findAllActorsOfType() const;

		/**
		 * Creates and returns a new actor of type T.
		 */
		template<typename T>
		T* createActor();

		/**
		 * Creates and returns a new Actor of the given type.
		 *
		 * Can return nullptr if no such type was registered.
		 */
		Actor* createActor(String const& type);
	protected:
		/**
		 * Renders the actors in this layer, in order of the actor list.
		 */
		void render(Renderer* renderer, Scene* scene) override;

		/**
		 * Removes the given actor from this layer, and then destroys the actor itself.
		 * Used internally by Engine/SceneManager to avoid deleting actors within critical loops.
		 */
		void destroyActor(Actor* actor);
	private:
		Vector<Unique<Actor>> actors;
	};

	template <typename T>
	T* ActorLayer::findActorOfType() const
	{
		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				return (T*)actor.get();
		}
		return nullptr;
	}

	template <typename T>
	T* ActorLayer::findActorOfType(String const& name) const
	{
		for (auto& actor : actors)
		{
			if (actor->name == name && dynamic_cast<T*>(actor.get()) != nullptr)
				return (T*)actor.get();
		}
		return nullptr;
	}

	template <typename T>
	Vector<T*> ActorLayer::findAllActorsOfType() const
	{
		Vector<T*> result;

		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				result.push_back((T*)actor.get());
		}
		return result;
	}

	template <typename T>
	T* ActorLayer::createActor()
	{
		static_assert(std::is_base_of<Actor, T>::value, "Can't add a new Actor if it isn't of type Actor");
		static_assert(!std::is_abstract<T>::value, "Can't add an abstract Actor!");
		
		T* actor = new T();
		actors.push_back(std::unique_ptr<T>(actor));
		return actor;
	}
}
