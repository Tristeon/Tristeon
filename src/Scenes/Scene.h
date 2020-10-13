#pragma once
#include <Serialization/Serializable.h>
#include <Scenes/Layers/Layer.h>

#include <Standard/List.h>
#include <Standard/Unique.h>

namespace Tristeon
{
	/**
	 * Scene is a class that describes the current 'scene' or 'level' within the game.
	 * Its core design revolves around layers, which are each constructed of their own set of data,
	 * used to render things such as tilemaps or sprites (actors).
	 * Common layers include:
	 * - TileLayer -> Renders a tilemap with a given tileset
	 * - ActorLayer -> Contains actor objects which are free to move off the grid at will.
	 */
	class Scene final : public Serializable
	{
		friend class Engine;
		friend class SceneManager;

	public:
		/**
		 * Creates an empty scene with a default camera & hud.
		 */
		Scene() = default;
		~Scene() = default;

		DELETE_COPY(Scene);
		DEFAULT_MOVE(Scene);

		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Gets a layer using the given index.
		 *
		 * \exception invalid_argument Throws if the index is less than 0 or more than layerCount() - 1
		 */
		[[nodiscard]] Layer* layerAt(const unsigned long long& index) const;

		/**
		 * Finds a layer using the given name.
		 * Returns nullptr if no layer is found.
		 */
		[[nodiscard]] Layer* findLayer(const String& name) const;

		/**
		 * Adds a new layer of the given type and returns it.
		 */
		template<typename T>
		[[nodiscard]] T* createLayer();

		/**
		 * Adds a new layer of the given type and returns it.
		 *
		 * Can return nullptr if no such type was registered.
		 */
		[[nodiscard]] Layer* createLayer(const String& type);
		
		/**
		 * Finds the first layer of the given type.
		 * Returns nullptr if no layer is found.
		 *
		 * Compilation fails if T does not derive from Layer.
		 */
		template<typename T>
		[[nodiscard]] T* findLayerOfType();

		/**
		 * Finds the first layer of the given type and name.
		 * Returns nullptr if no layer is found.
		 */
		template<typename T>
		[[nodiscard]] T* findLayerOfType(const String& name) const;

		/**
		 * Returns a list with all the layers of the given type.
		 * Returns an empty list if no layer is found.
		 */
		template<typename T>
		[[nodiscard]] List<T*> findLayersOfType();

		/**
		 * Returns the amount of layers this scene contains.
		 */
		[[nodiscard]] ull layerCount() const;

		/**
		 * Queues the layer up for destruction.
		 * Destruction is processed after each critical loop in the Engine.
		 */
		void destroyLayer(Layer* layer);

		/**
		 * Sets the index of the given layer. This removes the layer from its current position in the list and inserts it at the given index.
		 * Other layers will be moved down or up because of this.
		 */
		void setIndex(Layer* layer, const ull& index);

		/**
		 * Returns the index of the given layer.
		 * If the layer is nullptr or if the layer doesn't exist in the scene, it will return Scene::layerCount() instead.
		 */
		[[nodiscard]] ull indexOf(Layer* layer);

		/**
		 * The scene's filepath, local to the project.
		 */
		[[nodiscard]] String path() const { return _path; }

		/**
		 * Gets the Scene's file name.
		 */
		[[nodiscard]] String name() const { return _name; }
	private:
		/**
		 * Used internally by Engine to avoid deleting Layers within critical loops.
		 */
		void safeCleanup();
		List<Layer*> _destroyedLayers{};
		
		List<Unique<Layer>> _layers;
		String _name;
		String _path;
	};

	template <typename T>
	T* Scene::createLayer()
	{
		static_assert(std::is_base_of<Layer, T>::value, "Can't add a new Layer if it isn't of type Layer");
		static_assert(!std::is_abstract<T>::value, "Can't add an abstract Layer!");
		
		T* newLayer = new T();
		_layers.add(std::unique_ptr<T>(newLayer));
		return newLayer;
	}

	template <typename T>
	T* Scene::findLayerOfType()
	{
		for (auto i = 0; i < _layers.size(); i++)
		{
			if (dynamic_cast<T*>(_layers[i].get()) != nullptr)
				return dynamic_cast<T*>(_layers[i].get());
		}
		return nullptr;
	}

	template <typename T>
	T* Scene::findLayerOfType(String const& name) const
	{
		for (auto i = 0; i < _layers.size(); i++)
		{
			if (name == _layers[i]->name && dynamic_cast<T*>(_layers[i].get()) != nullptr)
				return dynamic_cast<T*>(_layers[i].get());
		}
		return nullptr;
	}

	template <typename T>
	List<T*> Scene::findLayersOfType()
	{
		List<T*> result;

		for (auto i = 0; i < _layers.size(); i++)
		{
			if (dynamic_cast<T*>(_layers[i].get()) != nullptr)
				result.add(dynamic_cast<T*>(_layers[i].get()));
		}
		return result;
	}
}
