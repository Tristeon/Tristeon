#pragma once
#include "Serialization/InstancedSerializable.h"
#include "Utils/TypeDefinitions.h"
#include <Rendering/Framebuffer.h>

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

		/**
		 * Queues the layer up for destruction.
		 * Destruction is processed after each critical loop in the Engine.
		 */
		void destroy();
	protected:
		/**
		 * Renders this layer to the currently bound framebuffer.
		 * Layers are free to implement this in whichever way they like.
		 */
		virtual void render(const Framebuffer& framebuffer) = 0;

		/**
		 * A virtual function that can be used by Layers to safely cleanup/destroy objects that might be subject to callback and/or other loops.
		 *
		 * This function is called in-between iteration loops to prevent breaking critical loops.
		 */
		virtual void safeCleanup() { /* Empty */ }
	};
}