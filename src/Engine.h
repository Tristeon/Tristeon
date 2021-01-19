#pragma once
#include <Utils/Singleton.h>

#include "Audio/Audio.h"
#include "Physics/PhysicsWorld.h"
#include "Rendering/Renderer.h"

int main(int argc, char** argv);

namespace Tristeon
{
	/**
	 * Engine is the absolute core of Tristeon. It owns engine subsystems such as Renderer, GameView, Physics, etc.
	 * It manages the creation and destruction of the engine subsystems. Subsystems may be accessible through their own Singleton instances.
	 */
	class Engine final : private Singleton<Engine>
	{
		friend Singleton<Engine>;
		friend class Actor;
		friend class Behaviour;
		friend int ::main(int argc, char** argv);
	public:
		/**
		 * Enables or disables the play-mode, usually adjusted by the editor.
		 */
		static void setPlayMode(const bool& enabled);

		/**
		 * Gets if the engine is in play-mode or not.
		 */
		[[nodiscard]] static bool playMode();
		
	private:
		/**
		 * Runs the engine, usually called by the main() function.
		 */
		void run();
		
		bool _playMode = false;
		Unique<Renderer> _renderer = nullptr;
		Unique<PhysicsWorld> _physics = nullptr;
		Unique<Audio> _audio = nullptr;
	};
}