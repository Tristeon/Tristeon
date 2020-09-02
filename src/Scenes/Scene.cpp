#include "Scene.h"
#include <stdexcept>

#include <Scenes/Layers/Layer.h>

#include "Serialization/TypeRegister.h"

namespace Tristeon
{
	Scene::Scene()
	{
		camera = std::make_unique<Camera>();
	}

	json Scene::serialize()
	{
		json j;
		j["typeID"] = TRISTEON_TYPENAME(Scene);
		j["camera"] = camera->serialize();

		json serializedLayers = json::array_t();
		for (auto& layer : layers)
			serializedLayers.push_back(layer->serialize());
		j["layers"] = serializedLayers;
		return j;
	}

	void Scene::deserialize(json j)
	{
		camera->deserialize(j.value("camera", json()));

		layers.clear(); //TODO: Could detect and reuse existing layers as opposed to clearing every time
		for (auto serializedLayer : j.value("layers", json::array_t()))
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedLayer["typeID"]);
			serializable->deserialize(serializedLayer);
			layers.add(Unique<Layer>((Layer*)serializable.release()));
		}
	}

	Layer* Scene::getLayer(unsigned int const& index) const
	{
		if (index < 0 || index > layers.size() - 1)
			throw std::invalid_argument("Index must be more than 0 and less than the amount of layers");

		return layers[index].get();
	}

	Layer* Scene::findLayer(std::string const& name) const
	{
		for (const auto& layer : layers)
		{
			if (layer->name == name)
				return layer.get();
		}
		return nullptr;
	}

	Layer* Scene::addLayer(String const& type)
	{
		Unique<Serializable> serializable = TypeRegister::createInstance(type);
		auto* layer = dynamic_cast<Layer*>(serializable.get());
		if (layer != nullptr)
		{
			layer = (Layer*)serializable.release();
			layers.add(std::unique_ptr<Layer>(layer));
			return layer;
		}
		return nullptr;
	}

	unsigned int Scene::getLayerCount() const
	{
		return layers.size();
	}

	void Scene::destroyLayer(Layer* layer)
	{
		int const index = indexOf(layer);
		if (index == layers.size())
			return;
		layers[index].reset();
		layers.removeAt(index);
	}

	Camera* Scene::getCamera() const
	{
		return camera.get();
	}

	void Scene::setIndex(Layer* layer, int const& i)
	{
		ull const old = indexOf(layer);
		if (old == layers.size())
			return;

		auto unique = std::move(layers[old]);
		layers.removeAt(old);
		layers.insert(i, std::move(unique));
	}

	int Scene::indexOf(Layer* layer)
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (layers[i].get() == layer)
			{
				return i;
			}
		}
		return -1;
	}
}
