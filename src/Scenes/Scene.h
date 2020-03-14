#pragma once
#include <memory>
#include <vector>

#include <Scenes/Camera.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
#include <TypeDefinitions.h>

#include <Scenes/HUD.h>

namespace Tristeon
{
	class SceneManager;
	class Layer;
	class Engine;
	class GameView;

	template<typename T>
	using IsLayer = std::enable_if_t<std::is_base_of<Layer, T>::value, T>;
	
	/**
	 * Scene is a class that describes the current 'scene' or 'level' within the game.
	 * Its core design revolves around layers, which are each constructed of their own set of data,
	 * used to render things such as tilemaps or sprites (actors).
	 * Common layers include:
	 * - TileLayer -> Renders a tilemap with a given tileset
	 * - ActorLayer -> Contains actor objects which are free to move off the grid at will.
	 * 
	 * Within Tristeon, Scene is constructed out of
	 * - A camera
	 * - A HUD
	 */
	class Scene final : public Serializable
	{
		friend Engine;
		friend GameView;
		friend SceneManager;

		REGISTER_TYPE_H(Scene)

	public:
		/**
		 * Creates an empty scene with a default camera & hud.
		 */
		Scene();
		~Scene() = default;

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets a layer using the given index.
		 *
		 * \exception invalid_argument Throws if the index is less than 0 or more than getLayerCount()
		 */
		Layer* getLayer(unsigned int const& index) const;

		/**
		 * Finds a layer using the given name.
		 * Returns nullptr if no layer is found.
		 */
		Layer* findLayer(std::string const& name) const;

		/**
		 * Finds the first layer of the given type.
		 * Returns nullptr if no layer is found.
		 *
		 * Compilation fails if T does not derive from Layer.
		 */
		template<typename T>
		IsLayer<T>* findLayerOfType();

		/**
		 * Returns a vector with all the layers of the given type.
		 * Returns an empty vector if no layer is found.
		 *
		 * Compilation fails if T does not derive from Layer.
		 */
		template<typename T>
		Vector<IsLayer<T>*> findLayersOfType();

		/**
		 * Returns the amount of layers this scene contains.
		 */
		unsigned int getLayerCount() const;

		/**
		 * Returns the Scene HUD.
		 */
		HUD* getHUD() const;

		/**
		 * Returns the Scene Camera.
		 */
		Camera* getCamera() const;
	private:
		void update();

		Unique<Camera> camera = nullptr;
		Unique<HUD> hud = nullptr;
		Vector<Unique<Layer>> layers;
	};

	template <typename T>
	IsLayer<T>* Scene::findLayerOfType()
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (dynamic_cast<T>(layers[i].get()) != nullptr)
				return (T*)layers[i].get();
		}
		return nullptr;
	}

	template <typename T>
	Vector<IsLayer<T>*> Scene::findLayersOfType()
	{
		Vector<T*> result;

		for (size_t i = 0; i < layers.size(); i++)
		{
			if (dynamic_cast<T>(layers[i].get()) != nullptr)
				result.push_back(dynamic_cast<T>(layers[i].get()));
		}
		return result;
	}
}