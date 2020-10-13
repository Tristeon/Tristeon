#pragma once
#include "Scenes/Actors/Actor.h"
#include "Callbacks/IDrawGizmos.h"

#include <Utils/ClassDefaults.h>
#include <Serialization/TypeRegister.h>

#include "Math/Vector.h"

namespace Tristeon
{
	/**
	 * Camera describes the way the game is viewed.
	 * The camera can be moved and zoomed as to adjust the current game view.
	 *
	 * The values of this class are used internally to adjust the way the world is rendered,
	 * but the variables may be changed at any time by the user.
	 */
	class Camera : public Actor, public IDrawGizmos
	{
		friend class Renderer;
	public:
		Camera();
		explicit Camera(const bool& registerSelf);
		~Camera();

		DELETE_COPY(Camera);
		DEFAULT_MOVE(Camera);

		/**
		 * The render-size of the camera, this is a multiplier to the resolution and can be used to make the camera cover only a part of the screen.
		 */
		Vector screenSize = Vector(1, 1);
		/**
		 * The coordinates of the bottom-left corner of the camera on the screen, these are in range -1..1 in where -1, 1 is on the bottom-left and 1,1 is on the top-right.
		 */
		Vector screenCoordinates = Vector(-1, -1);
		/**
		 * Determines whether the camera renders to the screen or not.
		 * If false, the camera's texture isn't used by the renderer but can be used for other purposes.
		 * If false, the camera is rendered before anything else to allow it to be used as a texture afterwards.
		 */
		bool renderToScreen = true;
		/**
		 * Override the resolution the camera renders at.
		 * This is only used if renderToScreen == false.
		 */
		VectorI overrideResolution{ 800, 800 };
		/**
		 * The zoom of the camera. This value is 1 by default but can be adjusted to zoom in/out
		 * of the scene without having to adjust the size variable.
		 */
		float zoom = 1;
		
		json serialize() override;
		void deserialize(json j) override;
		void drawGizmos() override;

		/**
		 * Get all camera instances.
		 * This is the same as calling Collector<Camera>::all();
		 */
		[[nodiscard]] static List<Camera*> cameras() { return Collector<Camera>::all(); }

		/**
		 * Returns the OpenGL frame buffer color texture.
		 * This texture is rendered to by the camera and can be used in other rendering scenarios.
		 *
		 * If referenced, the user must keep in mind that the texture could become invalid after each frame, as the framebuffer gets rebuilt when the screen gets resized.
		 */
		[[nodiscard]] unsigned int framebufferTexture() const { return _fboTexture; }

		/**
		 * Returns a copy of the framebuffer structure with the camera's current framebuffer properties.
		 */
		[[nodiscard]] Framebuffer framebuffer();
	private:
		//Keep track of old values so we can update the framebuffer if needed
		Vector _lastScreenSize{};
		VectorI _lastWindowSize{};

		//Framebuffer
		unsigned int _fbo = 0;
		unsigned int _fboTexture = 0;
		bool _valid = false;
		
		/**
		 * Creates a framebuffer and its corresponding color texture.
		 */
		void buildFramebuffer();
		/**
		 * Binds the framebuffer for use.
		 */
		void updateFramebuffer();
		/**
		 * Draws the camera's framebuffer texture to the screen.
		 */
		void drawToScreen() const;
	};

	REGISTER_TYPE(Camera);
}