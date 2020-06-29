#pragma once
#ifndef TRISTEON_EDITOR
#include "Window.h"

#include <GL/glew.h>
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
		void _setFullscreen(bool const& value) override;
		void _close() override;
		void _setClearColour(Colour const& colour) override;
		bool _closingDown() override;
		void _setWindowTitle(std::string const& value) override;
		Vector2 _screenToWorld(Vector2Int const& screenPoint) override;
		Vector2Int _worldToScreen(Vector2 const& worldPoint) override;

	private:
		void setupCallbacks();
		
		static void errorCallback(int error, const char* description);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		
		GLFWwindow* window = nullptr;
		unsigned int w = 0, h = 0;
		bool fullscreen;
	};
}
#endif