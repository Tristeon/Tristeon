#pragma once
#include <Math/Vector2Int.h>
#include <Serialization/Serializable.h>
#include <Serialization/TypeRegister.h>

namespace Tristeon
{
	/**
	 * Camera describes the way the game is viewed.
	 * The camera can be moved and zoomed as to adjust the current game view.
	 *
	 * The values of this class are used internally to adjust the way the world is rendered,
	 * but the variables may be changed at any time by the user, usually through Camera::main()
	 */
	class Camera : public Serializable
	{
		REGISTER_TYPE_H(Camera)
	public:
		/**
		 * The position of the camera
		 */
		Vector2Int position;
		/**
		 * The render-size of the camera, this can be different than the resolution to adjust for scaling behaviour
		 * TODO: Implement more complex scaling behaviour 
		 */
		Vector2Int size = Vector2Int(1920, 1080);
		/**
		 * THe zoom of the camera. This value is 1 by default but can be adjusted to zoom in/out
		 * of the scene without having to adjust the size variable.
		 */
		float zoom = 0;
		
		json serialize() override;
		void deserialize(json j) override;

		/**
		 * Returns the main camera owned by the current scene.
		 */
		static Camera* main();
	};
}
