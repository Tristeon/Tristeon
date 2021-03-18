#pragma once
#include <memory>
#include <Utils/Singleton.h>
#include <Rendering/Camera.h>
#include <Utils/ClassDefaults.h>

namespace Tristeon
{
	/**
	 * The Renderer is an engine subysystem that manages rendering objects within a Scene.
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer();
		~Renderer();

		DELETE_COPY(Renderer);
		DEFAULT_MOVE(Renderer);
		
		/**
		 * Renders the current scene to the given framebuffer. The render function executes a set of steps in the following order:
		 *
		 * For each Camera:
			* Pass camera data to each shader
			* Render composite lights for the camera's viewport
			* Bind composite light textures
			*
			* Set viewport
			* Render each layer through its virtual render() function
			* Render Grid/Gizmos
		 * 
		 * Bind default framebuffer
		 * Set viewport to fullscreen
		 * Render each camera to the screen
		 * Clear Gizmos
		 */
		void render(const unsigned int& pFramebuffer) const;

#ifdef TRISTEON_EDITOR
		[[nodiscard]] static Camera* editorCamera()
		{
			if (!instance())
				return nullptr;
			return instance()->_editorCamera.get();
		}
		bool _editorLightingDisabled = false;
		bool _editorGizmosEnabled = true;
		bool _editorGridEnabled = true;
	private:
		std::unique_ptr<Camera> _editorCamera = nullptr;
#endif

	private:
		void passCameraData(Camera* pCamera) const;
		
		void renderOffline(Camera* pCamera) const;
		void renderOnscreen(const unsigned int& pFramebuffer, const List<Camera*>& pCameras) const;
		
		unsigned int _dummyVao = 0;
	};
}
