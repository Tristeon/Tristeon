#pragma once
#include <memory>

#include <QObject>

#include <Rendering/GameView.h>
#include <Rendering/Renderer.h>

#include <TypeDefinitions.h>

namespace Tristeon
{
	/**
	 * Engine is the absolute core of Tristeon. It owns engine subsystems such as Renderer, GameView, Physics, etc.
	 * It manages the creation and destruction of the engine subsystems. Subsystems may be accessible through their own Singleton instances.
	 */
	class Engine final : public QObject
	{
		friend GameView;
	public:
		/**
		 * Called in main.cpp, do not call again.
		 */
		Engine();

		Engine(Engine const& other) = delete;
		void operator=(Engine const& other) = delete;
	private:
		GameView* _view = nullptr; //Non-owning, it's created and destroyed by the Qt loader. GameView sets this variable in its constructor.
		Unique<Renderer> _renderer = nullptr;

		QTimer* timer = nullptr; //Qt objects can't be unique_ptr so we delete it manually

		/**
		 * Creates the engine subsystems.
		 * This function is called by GameView because Qt requires us to wait till QtApplication::exec() is called.
		 */
		void initialize();

		/**
		 * Called every frame, runs the subsystem updates and renders the current scene.
		 */
		void update();
	};
}