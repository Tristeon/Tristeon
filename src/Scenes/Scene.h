#pragma once
#include <memory>
#include <vector>

#include <Scenes/Camera.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>
#include <TypeDefinitions.h>

#include <Scenes/HUD.h>

#include "Layers/Layer.h"

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
		 * Adds a new layer of the given type and returns it.
		 */
		template<typename T>
		T* addLayer();

		/**
		 * Adds a new layer of the given type and returns it.
		 *
		 * Can return nullptr if no such type was registered.
		 */
		Layer* addLayer(String const& type);
		
		/**
		 * Finds the first layer of the given type.
		 * Returns nullptr if no layer is found.
		 *
		 * Compilation fails if T does not derive from Layer.
		 */
		template<typename T>
		IsLayer<T>* findLayerOfType();

		/**
		 * Finds the first layer of the given type and name.
		 * Returns nullptr if no layer is found.
		 *
		 * Compilation fails if T does not derive from Layer.
		 */
		template<typename T>
		IsLayer<T>* findLayerOfType(String const& name) const;

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
		 * Removes the layer from the Scene and destroys it.
		 * \param layer After this function, layer will point to invalid memory.
		 */
		void destroyLayer(Layer* layer);
		
		/**
		 * Returns the Scene HUD.
		 */
		HUD* getHUD() const;

		/**
		 * Returns the Scene Camera.
		 */
		Camera* getCamera() const;

		/**
		 * Sets the index of the given layer. This removes the layer from its current position in the list and inserts it at the given index.
		 * Other layers will be moved down or up because of this.
		 */
		void setIndex(Layer* layer, int const& i);

		/**
		 * Returns the index of the given layer. -1 if the layer isnt part of the scene.
		 */
		int indexOf(Layer* layer);

		/**
		 * Gets the filepath if the Scene was loaded through SceneManager::load().
		 */
		String getPath() const { return path; }

		/**
		 * Gets the file name if the Scene was loaded through SceneManager::load().
		 */
		String getName() const { return name; }
	private:
		Unique<Camera> camera = nullptr;
		Unique<HUD> hud = nullptr;
		Vector<Unique<Layer>> layers;
		String name;
		String path;
	};

	template <typename T>
	T* Scene::addLayer()
	{
		static_assert(std::is_base_of<Layer, T>::value, "Can't add a new Layer if it isn't of type Layer");
		static_assert(!std::is_abstract<T>::value, "Can't add an abstract Layer!");
		
		T* newLayer = new T();
		layers.push_back(std::unique_ptr<T>(newLayer));
		return newLayer;
	}

	template <typename T>
	IsLayer<T>* Scene::findLayerOfType()
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (dynamic_cast<T*>(layers[i].get()) != nullptr)
				return dynamic_cast<T*>(layers[i].get());
		}
		return nullptr;
	}

	template <typename T>
	IsLayer<T>* Scene::findLayerOfType(String const& name) const
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (name == layers[i]->name && dynamic_cast<T*>(layers[i].get()) != nullptr)
				return dynamic_cast<T*>(layers[i].get());
		}
		return nullptr;
	}

	template <typename T>
	Vector<IsLayer<T>*> Scene::findLayersOfType()
	{
		Vector<T*> result;

		for (size_t i = 0; i < layers.size(); i++)
		{
			if (dynamic_cast<T*>(layers[i].get()) != nullptr)
				result.push_back(dynamic_cast<T*>(layers[i].get()));
		}
		return result;
	}
}
