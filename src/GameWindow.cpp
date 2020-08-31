#ifndef TRISTEON_EDITOR
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
			throw std::exception("Failed to initialize GLFW");
		glfwSetErrorCallback(&GameWindow::errorCallback);
		
		window = glfwCreateWindow(1920, 1080, "Tristeon", NULL, NULL);
		if (!window)
			throw std::exception("Failed to create GLFW window");
		glfwMakeContextCurrent(window);

		//Load OGL
		if (!gladLoadGL())
		{

			std::cout << "Error initializing glad." << std::endl;
			throw std::exception("Failed to initialize glad");
		}
		std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";
		
		fullscreen = Project::Graphics::fullscreen();
		setFullscreen(Project::Graphics::fullscreen());

		setupCallbacks();

		glClearColor(0, 0, 0, 1);

		//Enable culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, 1920, 1080);

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
		glfwDestroyWindow(window);
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
		glfwSwapBuffers(window);
	}

	unsigned GameWindow::_windowWidth()
	{
		return w;
	}

	unsigned GameWindow::_windowHeight()
	{
		return h;
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
		return fullscreen;
	}

	void GameWindow::_setFullscreen(bool const& value)
	{
		fullscreen = value;
		
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		
		if (value)
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
			glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);

		glViewport(0, 0, mode->width, mode->height);
	}

	void GameWindow::_close()
	{
		glfwSetWindowShouldClose(window, true);
	}

	void GameWindow::_setClearColour(Colour const& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	bool GameWindow::_closingDown()
	{
		return glfwWindowShouldClose(window);
	}

	void GameWindow::_setWindowTitle(std::string const& value)
	{
		glfwSetWindowTitle(window, value.data());
	}

	Vector2 GameWindow::_screenToWorld(Vector2Int const& screenPoint)
	{
		//Adjust for center
		Vector2 const halfSize = Vector2(gameWidth() / 2.0f, gameHeight() / 2.0f);
		Vector2 result = static_cast<Vector2>(screenPoint) - halfSize;

		//Adjust for camera
		result *= 1.0f / Camera::main()->zoom;
		result += Camera::main()->position;
		return result;
	}

	Vector2Int GameWindow::_worldToScreen(Vector2 const& worldPoint)
	{
		Vector2 point = worldPoint;
		point -= Camera::main()->position;
		point /= (1.0f / Camera::main()->zoom);

		Vector2 const halfSize = Vector2(gameWidth() / 2.0f, gameHeight() / 2.0f);
		point += halfSize;

		return Vector2Int(point);
	}

	void GameWindow::setupCallbacks()
	{
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

		glfwSetKeyCallback(window, keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
	}

	void GameWindow::errorCallback(int error, const char* description)
	{
		std::cout << "GLFW Error: " << error << ". Description: " << description << "\n";
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