#pragma once
#include "Serialization/InstancedSerializable.h"
#include "TypeDefinitions.h"

namespace Tristeon
{
	/**
	 * Layers are core to the way Scenes are defined.
	 * The Scene has a list of layers, each layer is rendered and updated in the order of said list.
	 * This base-class defines the minimal features of a Layer (a render and update function and a name).
	 */
	class Layer : public InstancedSerializable
	{
		friend class Renderer;
		friend class Scene;
	public:
		Layer() = default;
		virtual ~Layer() noexcept = default;

		DELETE_COPY(Layer);
		DEFAULT_MOVE(Layer);
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * The name of the layer.
		 * The name can be used to identify and lookup any layer using Scene::getLayer(string)
		 */
		String name = "";

	protected:
		/**
		 * Renders this layer to the GameView.
		 * Layers are free to implement this in whichever way they like.
		 */
		virtual void render(Renderer* renderer, Scene* scene) = 0;
	};
}