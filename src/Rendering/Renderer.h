#pragma once
#include <Utils/Singleton.h>
#include <Rendering/Shader.h>
#include "TypeDefinitions.h"

namespace Tristeon
{
	class Scene;
	class HUD;
	class GameView;

	/**
	 * The Renderer is an engine subysystem that manages rendering objects within a Scene.
	 */
	class Renderer final : public Singleton<Renderer>
	{
		friend GameView;
		friend Shader;
	public:
		static bool showGrid;
	private:
		/**
		 * Renders the given scene, runs a prepass for each shader in Renderer::prepass first
		 */
		void renderScene(Scene* scene);

		/**
		 * Renders the HUD and all of its child elements.
		 */
		void renderHUD(HUD* hud);

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
