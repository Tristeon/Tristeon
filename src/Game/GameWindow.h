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
		void _setVsync(const bool& value) override;
		void _setResolution(const VectorU& resolution) override;
		void _setWindowMode(const Project::Graphics::WindowMode& value) override;
		void _setDisplay(const unsigned& monitor) override;
		unsigned int _currentDisplay() override;
		void _close() override;
		void _setClearColour(const Colour& colour) override;
		bool _closingDown() override;
		void _setWindowTitle(const String& value) override;
		Vector _screenToWorld(const VectorU& screenPoint, Camera* camera) override;
		VectorU _worldToScreen(const Vector& worldPoint, Camera* camera) override;

	private:
		void setupCallbacks();

		void updateDisplay(GLFWmonitor* monitor, const Project::Graphics::WindowMode& windowmode);
		
		static void errorCallback(int error, const char* description);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		static void joystickCallback(int jid, int event);
		static void pollJoystick();
		static void monitorCallback(GLFWmonitor* monitor, int event);
		
		GLFWwindow* _window = nullptr;
		unsigned int _width = 0, _height = 0;
	};
}
#endif