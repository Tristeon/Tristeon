#pragma once
#include "ICallback.h"

namespace Tristeon
{
	class Scene;

	class ISceneLoaded : ICallback<ISceneLoaded>
	{
	public:
		virtual void sceneLoaded(Scene* scene) = 0;
	};
}