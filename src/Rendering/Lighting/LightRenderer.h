#pragma once

namespace Tristeon
{
	class Camera;

	class LightRenderer final
	{
		friend class Renderer;

		static void renderComposite(Camera* pCamera);
		static void bindCompositeTextures(Camera* pCamera);
		static void passLights();
	};
}