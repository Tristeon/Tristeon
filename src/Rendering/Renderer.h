#pragma once
#include <memory>
#include <Utils/Singleton.h>

#include "Actors/Camera.h"

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
