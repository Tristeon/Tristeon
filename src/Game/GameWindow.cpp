#ifndef TRISTEON_EDITOR
#include <exception>

#include "glad/glad.h"
#include "GameWindow.h"

#include "Project.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Rendering/Renderer.h"
#include "Scenes/Scene.h"
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
		
		_fullscreen = Project::Graphics::fullscreen();
		setFullscreen(Project::Graphics::fullscreen());

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

	bool GameWindow::_isFullscreen()
	{
		return _fullscreen;
	}

	void GameWindow::_setFullscreen(const bool& value)
	{
		_fullscreen = value;
		
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		
		if (value)
			glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
			glfwSetWindowMonitor(_window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);

		glViewport(0, 0, mode->width, mode->height);
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

	Vector GameWindow::_screenToWorld(const VectorI& screenPoint, Camera* camera)
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

	VectorI GameWindow::_worldToScreen(const Vector& worldPoint, Camera* camera)
	{
		Vector point = worldPoint;
		point -= camera->position;
		point /= (1.0f / camera->zoom);

		point += ((camera->screenCoordinates + Vector::one()) / 2.0f) * Window::gameSize();
		point += (Vector)Window::gameSize() * camera->screenSize / 2.0f; //Adjust for center

		return VectorI(point);
	}

	void GameWindow::setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);

		glfwSetKeyCallback(_window, keyCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetCursorPosCallback(_window, cursorPosCallback);
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
		//TODO: Handle Right shift, control, alt and super
		if (action == GLFW_PRESS)
			Keyboard::onPress((Keyboard::Key)key);
		else if (action == GLFW_RELEASE)
			Keyboard::onRelease((Keyboard::Key)key);
	}

	void GameWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		//TODO: Mouse button mapping might be incorrect
		if (action == GLFW_PRESS)
			Mouse::onPress((Mouse::MouseButton)button);
		else if (action == GLFW_RELEASE)
			Mouse::onRelease((Mouse::MouseButton)button);
	}

	void GameWindow::cursorPosCallback(GLFWwindow* window, double x, double y)
	{
		Mouse::onMove({ (int)x, (int)y });
	}
}
#endif