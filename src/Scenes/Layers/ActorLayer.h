#pragma once
#include "Layer.h"

#include <Actors/Actor.h>
#include <Serialization/TypeRegister.h>
#include <vector>

namespace Tristeon
{
	class Shader;
	class SceneManager;

	template<typename T>
	using IsActor = std::enable_if_t<std::is_base_of<Actor, T>::value, T>;
	
	/**
	 * The actor layer contains a list of actors.
	 *
	 * Actors are objects which are free to move, rotate and scale around, unlimited by the tile-grid.
	 * The layer updates & renders them, and acts as a way for other classes to freely interact with actors.
	 */
	class ActorLayer : public Layer
	{
		REGISTER_TYPE_H(ActorLayer);

		friend SceneManager;
	public:
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
		 * 
		 * Compilation fails if T does not derive from Actor.
		 */
		template<typename T>
		IsActor<T>* findActorOfType() const;

		/**
		 * Returns a vector of all the actors of the given type within this layer.
		 *
		 * Returns an empty vector if no actors were found.
		 * 
		 * Compilation fails if T does not derive from Actor.
		 */
		template<typename T>
		Vector<IsActor<T>*> findAllActorsOfType() const;
	protected:
		/**
		 * Renders the actors in this layer, in order of the actor list.
		 */
		void render(Renderer* renderer, Scene* scene) override;

		/**
		 * Sends an update call to each actor.
		 */
		void update() override;
	private:
		Vector<Unique<Actor>> actors;
	};

	template <typename T>
	IsActor<T>* ActorLayer::findActorOfType() const
	{
		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				return (T*)actor.get();
		}
		return nullptr;
	}

	template <typename T>
	Vector<IsActor<T>*> ActorLayer::findAllActorsOfType() const
	{
		Vector<T*> result;

		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get()) != nullptr)
				result.push_back((T*)actor.get());
		}
		return result;
	}
}
