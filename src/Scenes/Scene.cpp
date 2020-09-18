#include "Scene.h"
#include <stdexcept>
#include <Scenes/Layers/Layer.h>
#include "Serialization/TypeRegister.h"

namespace Tristeon
{
	json Scene::serialize()
	{
		json j = Serializable::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Scene);

		json serializedLayers = json::array_t();
		for (auto& layer : _layers)
			serializedLayers.push_back(layer->serialize());
		j["layers"] = serializedLayers;
		return j;
	}

	void Scene::deserialize(json j)
	{
		Serializable::deserialize(j);
		
		_layers.clear(); //TODO: Could detect and reuse existing layers as opposed to clearing every time
		for (const auto& serializedLayer : j.value("layers", json::array_t()))
		{
			Unique<Serializable> serializable = TypeRegister::createInstance(serializedLayer["typeID"]);
			serializable->deserialize(serializedLayer);
			_layers.add(Unique<Layer>(dynamic_cast<Layer*>(serializable.release())));
		}
	}

	Layer* Scene::layerAt(const unsigned int& index) const
	{
		if (index < 0 || index > _layers.size() - 1)
			throw std::invalid_argument("Index must be more than 0 and less than the amount of layers");

		return _layers[index].get();
	}

	Layer* Scene::findLayer(const String& name) const
	{
		for (const auto& layer : _layers)
		{
			if (layer->name == name)
				return layer.get();
		}
		return nullptr;
	}

	Layer* Scene::createLayer(const String& type)
	{
		Unique<Serializable> serializable = TypeRegister::createInstance(type);
		auto* layer = dynamic_cast<Layer*>(serializable.get());
		if (layer != nullptr)
		{
			layer = dynamic_cast<Layer*>(serializable.release());
			_layers.add(std::unique_ptr<Layer>(layer));
			return layer;
		}
		return nullptr;
	}

	unsigned int Scene::layerCount() const
	{
		return _layers.size();
	}

	void Scene::destroyLayer(Layer* layer)
	{
		ull const index = indexOf(layer);
		if (index == _layers.size())
			return;
		_layers[index].reset();
		_layers.removeAt(index);
	}

	void Scene::setIndex(Layer* layer, const ull& index)
	{
		ull const old = indexOf(layer);
		if (old == _layers.size())
			return;

		auto unique = std::move(_layers[old]);
		_layers.removeAt(old);
		_layers.insert(index, std::move(unique));
	}

	ull Scene::indexOf(Layer* layer)
	{
		for (size_t i = 0; i < _layers.size(); i++)
		{
			if (_layers[i].get() == layer)
			{
				return i;
			}
		}
		return -1;
	}
}