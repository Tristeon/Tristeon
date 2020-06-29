#pragma once
#include <Utils/Singleton.h>
#include "TypeDefinitions.h"

namespace Tristeon
{
	class Shader;
	class Scene;
	class HUD;

	/**
	 * The Renderer is an engine subysystem that manages rendering objects within a Scene.
	 */
	class Renderer final : public Singleton<Renderer>
	{
		friend Shader;
	public:
		static bool showGrid;
		/**
		 * Renders the current scene to the given framebuffer. The render function executes a set of steps in the following order:
		 *
		 * 1) Shader prepass sends camera data to each shader
		 * 2) Render each layer individually
		 * 3) Render Scene HUD
		 * 4) Render Grid if enabled
		 * 5) Render Gizmos if enabled
		 */
		void render(unsigned int const& framebuffer);

	private:
		/**
		 * Registers a shader to the prepass call.
		 *
		 * Prepass sends standard engine values to the shader such as camera & lighting data.
		 */
		static void registerPrePassShader(Shader* shader);

		/**
		 * Deregisters a shader from the prepass call.
		 */
		static void deregisterPrePassShader(Shader* shader);

		static Vector<Shader*> prepass;
	};
}
