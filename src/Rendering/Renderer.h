#pragma once
#include <memory>
#include <Utils/Singleton.h>
#include "Actors/Camera.h"
#include <Utils/ClassDefaults.h>

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
		Renderer();
		~Renderer();

		DELETE_COPY(Renderer);
		DEFAULT_MOVE(Renderer);
		
		/**
		 * Renders the current scene to the given framebuffer. The render function executes a set of steps in the following order:
		 *
		 * For each Camera:
			* Bind camera's framebuffer
			* Set viewport
			* Pass camera data to each shader
			* Render each layer through its virtual render() function
			* Render Gizmos
		 * 
		 * Bind default framebuffer
		 * Set viewport to fullscreen
		 * Render each camera to the screen
		 * Clear Gizmos
		 */
		void render(const unsigned int& framebuffer);

#ifdef TRISTEON_EDITOR
		[[nodiscard]] static Camera* editorCamera()
		{
			if (!instance())
				return nullptr;
			return instance()->_editorCamera.get();
		}
	private:
		std::unique_ptr<Camera> _editorCamera = nullptr;
#endif
	};
}