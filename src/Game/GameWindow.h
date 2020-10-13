#pragma once
#ifndef TRISTEON_EDITOR
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Tristeon
{
	class GameWindow : public Window
	{
	public:
		GameWindow();
		~GameWindow();
	protected:
		void _pollEvents() override;
		void _draw() override;
		unsigned _windowWidth() override;
		unsigned _windowHeight() override;
		unsigned _gameWidth() override;
		unsigned _gameHeight() override;
		bool _isFullscreen() override;
		void _setFullscreen(const bool& value) override;
		void _close() override;
		void _setClearColour(const Colour& colour) override;
		bool _closingDown() override;
		void _setWindowTitle(const String& value) override;
		Vector _screenToWorld(const VectorI& screenPoint, Camera* camera) override;
		VectorI _worldToScreen(const Vector& worldPoint, Camera* camera) override;

	private:
		void setupCallbacks();
		
		static void errorCallback(int error, const char* description);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		
		GLFWwindow* _window = nullptr;
		unsigned int _width = 0, _height = 0;
		bool _fullscreen;
	};
}
#endif