#ifndef TRISTEON_EDITOR
#include "glad/glad.h"
#include "GameWindow.h"

#include "Project.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Gamepad.h"
#include "Rendering/Renderer.h"
#include "Scenes/SceneManager.h"
#include "Utils/Colour.h"

namespace Tristeon
{
	GameWindow::GameWindow()
	{
		//Create window
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");
		glfwSetErrorCallback(&GameWindow::errorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_window = glfwCreateWindow(1920, 1080, "Tristeon", nullptr, nullptr);
		_width = 1920;
		_height = 1080;
		if (!_window)
			throw std::runtime_error("Failed to create GLFW window");
		glfwMakeContextCurrent(_window);

		//Load OGL
		if (!gladLoadGL())
		{
			Console::error("Error initializing glad");
			throw std::runtime_error("Failed to initialize glad");
		}
		Console::write("OpenGL " + std::to_string(GLVersion.major) + "." + std::to_string(GLVersion.minor));

		GameWindow::_setWindowMode(Project::Graphics::windowMode());
		GameWindow::_setDisplay(Project::Graphics::preferredDisplay());
		GameWindow::_setVsync(Project::Graphics::vsync());
		setupCallbacks();

		glClearColor(0, 0, 0, 1);

		//Enable culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, width(), height());

		//Enable transparency blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Disable multisampling because any form of aliasing messes with transparency in 2D
		glDisable(GL_MULTISAMPLE);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
	}

	GameWindow::~GameWindow()
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void GameWindow::_pollEvents()
	{
		glfwPollEvents();

		if (glfwGetWindowAttrib(_window, GLFW_FOCUSED))
			pollJoystick();
	}

	void GameWindow::_draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		auto* scene = SceneManager::current();
		if (scene != nullptr)
		{
			Renderer::instance()->render(0);
		}

		glfwSwapInterval(Project::Graphics::vsync());
		glfwSwapBuffers(_window);
	}

	unsigned GameWindow::_windowWidth()
	{
		return _width;
	}

	unsigned GameWindow::_windowHeight()
	{
		return _height;
	}

	unsigned GameWindow::_gameWidth()
	{
		return _windowWidth();
	}

	unsigned GameWindow::_gameHeight()
	{
		return _windowHeight();
	}

	void GameWindow::_setVsync(const bool& value)
	{
		glfwSwapInterval(value);
	}

	void GameWindow::_setResolution(const VectorU& resolution)
	{
		glfwSetWindowSize(_window, resolution.x, resolution.y);
		_width = resolution.x;
		_height = resolution.y;
		glViewport(0, 0, _width, _height);
	}

	void GameWindow::_setWindowMode(const Project::Graphics::WindowMode& value)
	{
		//Get available monitors
		int count = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if (!monitors)
			throw std::runtime_error("Failed to get GLFW monitors");

		//Attempt to get preferred monitor, otherwise use primary display
		GLFWmonitor* monitor = nullptr;
		if (Project::Graphics::preferredDisplay() >= count)
			monitor = glfwGetPrimaryMonitor();
		else 
			monitor = monitors[count - Project::Graphics::preferredDisplay() - 1];

		updateDisplay(monitor, value);
	}

	void GameWindow::_setDisplay(const unsigned& monitor)
	{
		//Get available monitors
		int count = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if (!monitors)
			throw std::runtime_error("Failed to get GLFW monitors");
		if (monitor >= count)
		{
			Console::warning("Invalid monitor " + std::to_string(monitor) + " selected, maximum number of monitors is " + std::to_string(count));
			return;
		}

		GLFWmonitor* selectedMonitor = monitors[count - monitor - 1];
		updateDisplay(selectedMonitor, Project::Graphics::windowMode());
		populateResolutions(monitor);
	}

	unsigned GameWindow::_currentDisplay()
	{
		int count = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if (!monitors)
			throw std::runtime_error("Failed to get GLFW monitors");

		GLFWmonitor* monitor = glfwGetWindowMonitor(_window);
		if (monitor == nullptr)
			return 0;

		for (unsigned int i = 0; i < count; i++)
		{
			if (monitor == monitors[i])
				return count - i - 1; //Convert to OS monitors 
		}
		throw std::runtime_error("Current display doesn't exist");
	}

	void GameWindow::_close()
	{
		glfwSetWindowShouldClose(_window, true);
	}

	void GameWindow::_setClearColour(Colour const& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	bool GameWindow::_closingDown()
	{
		return glfwWindowShouldClose(_window) == GLFW_TRUE;
	}

	void GameWindow::_setWindowTitle(const String& value)
	{
		glfwSetWindowTitle(_window, value.data());
	}

	Vector GameWindow::_screenToWorld(const VectorU& screenPoint, Camera* camera)
	{
		//Adjust for center
		auto result = static_cast<Vector>(screenPoint);

		result -= ((camera->screenCoordinates + Vector::one()) / 2.0f) * Window::gameSize();
		result -= (Vector)Window::gameSize() * camera->screenSize / 2.0f; //Adjust for center

		//Adjust for camera
		result *= 1.0f / camera->zoom;
		result += camera->position;
		return result;
	}

	VectorU GameWindow::_worldToScreen(const Vector& worldPoint, Camera* camera)
	{
		Vector point = worldPoint;
		point -= camera->position;
		point /= (1.0f / camera->zoom);

		point += ((camera->screenCoordinates + Vector::one()) / 2.0f) * Window::gameSize();
		point += (Vector)Window::gameSize() * camera->screenSize / 2.0f; //Adjust for center

		return VectorU(point);
	}

	void GameWindow::setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);

		glfwSetKeyCallback(_window, keyCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetCursorPosCallback(_window, cursorPosCallback);
		glfwSetJoystickCallback(joystickCallback);

		glfwSetMonitorCallback(monitorCallback);

		for (int jid = 0; jid < Gamepad::maximumGamepads; jid++)
		{
			Gamepad::gamepads[jid]._connected = glfwJoystickPresent(jid);

			if (Gamepad::gamepads[jid]._connected)
			{
				Gamepad::gamepads[jid]._name = glfwGetJoystickName(jid);
				Console::write("Gamepad detected: " + Gamepad::name(jid));
			}
		}
	}

	void GameWindow::updateDisplay(GLFWmonitor* monitor, const Project::Graphics::WindowMode& windowmode)
	{
		//Get monitor and adjust width/height appropriately
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (Project::Graphics::preferredResolution() != VectorU::zero())
		{
			_width = Project::Graphics::preferredResolution().x;
			_height = Project::Graphics::preferredResolution().y;
		}
		else
		{
			_width = mode->width;
			_height = mode->height;
		}

		switch (windowmode)
		{
			case Project::Graphics::WindowMode::Windowed:
			{
				//Enable windowed mode and decoration
				glfwSetWindowAttrib(_window, GLFW_DECORATED, GLFW_TRUE);
				glfwSetWindowMonitor(_window, nullptr, 0, 0, (int)_width, (int)_height, GLFW_DONT_CARE);

				//Move window over to the appropriate monitor
				int x, y;
				glfwGetMonitorPos(monitor, &x, &y);
				glfwSetWindowPos(_window, x, y);
				glfwSetWindowSize(_window, _width, _height);
				break;
			}
			case Project::Graphics::WindowMode::Borderless:
			{
				//Enable windowed mode but disable decoration
				glfwSetWindowAttrib(_window, GLFW_DECORATED, GLFW_FALSE);
				glfwSetWindowMonitor(_window, nullptr, 0, 0, (int)_width, (int)_height, GLFW_DONT_CARE);

				//Move window over to the appropriate monitor and maximize
				int x, y;
				glfwGetMonitorPos(monitor, &x, &y);
				glfwSetWindowPos(_window, x, y);
				glfwSetWindowSize(_window, _width, _height);
				glfwMaximizeWindow(_window);
				break;
			}
			case Project::Graphics::WindowMode::Fullscreen:
			{
				//Enable fullscreen mode
				glfwSetWindowMonitor(_window, monitor, 0, 0, (int)_width, (int)_height, GLFW_DONT_CARE);
				break;
			}
		}

		glViewport(0, 0, _width, _height);
	}

	void GameWindow::errorCallback(int error, const char* description)
	{
		Console::warning("GLFW Error: " + std::to_string(error) + ". Description: " + description);
	}

	void GameWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void GameWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
			Keyboard::onPress((Keyboard::Key)key);
		else if (action == GLFW_RELEASE)
			Keyboard::onRelease((Keyboard::Key)key);
	}

	void GameWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
			Mouse::onPress((Mouse::MouseButton)(button + 1));
		else if (action == GLFW_RELEASE)
			Mouse::onRelease((Mouse::MouseButton)(button + 1));
	}

	void GameWindow::cursorPosCallback(GLFWwindow* window, double x, double y)
	{
		Mouse::onMove({ (unsigned int)x, (unsigned int)y });
	}

	void GameWindow::joystickCallback(int jid, int event)
	{
		if (event == GLFW_DISCONNECTED)
		{
			Gamepad::gamepads[jid]._connected = false;
			Gamepad::clearGamepad(jid);
			Console::write("Gamepad disconnected: " + Gamepad::name(jid));
		}
		else if (event == GLFW_CONNECTED)
		{
			Gamepad::gamepads[jid]._connected = true;
			Gamepad::gamepads[jid]._name = glfwGetJoystickName(jid);
			Gamepad::clearGamepad(jid);
			Console::write("Gamepad connected: " + Gamepad::name(jid));
		}
	}

	void GameWindow::pollJoystick()
	{
		for (auto jid = 0; jid < (int)Gamepad::maximumGamepads; jid++)
		{
			if (!Gamepad::gamepads[jid]._connected)
				continue;

			GLFWgamepadstate state;
			if (glfwGetGamepadState(jid, &state))
			{
				for (unsigned int i = 0; i < Gamepad::Last + 1; i++)
				{
					if (state.buttons[i] != Gamepad::gamepads[jid]._buttons[i])
						Gamepad::buttonChanged(jid, (Gamepad::GamepadButton)i, state.buttons[i]);
				}

				Gamepad::gamepads[jid]._left = Vector(state.axes[0], state.axes[1]);
				Gamepad::gamepads[jid]._right = Vector(state.axes[2], state.axes[3]);
				Gamepad::gamepads[jid]._l2 = state.axes[4];
				Gamepad::gamepads[jid]._r2 = state.axes[5];
			}
		}
	}

	void GameWindow::monitorCallback(GLFWmonitor* monitor, int event)
	{
		if (event == GLFW_DISCONNECTED)
		{
			int count = 0;
			GLFWmonitor** monitors = glfwGetMonitors(&count);
			if (!monitors)
				throw std::runtime_error("Failed to get GLFW monitors");

			for (unsigned int i = 0; i < count; i++)
			{
				if (monitor == monitors[i] && i == Window::currentDisplay())
					Project::Graphics::setPreferredDisplay(0);
			}
		}
	}
}
#endif