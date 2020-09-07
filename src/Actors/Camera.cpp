#include "Camera.h"

#include <glad/glad.h>

#include "Window.h"
#include "Input/Keyboard.h"
#include "Rendering/Gizmos.h"
#include "Rendering/Shader.h"
#include "Utils/Console.h"

namespace Tristeon
{
	Camera::Camera()
	{
		Collector<Camera>::add(this);
	}

	Camera::Camera(const bool& registerSelf) : Actor(registerSelf)
	{
		if (registerSelf)
			Collector<Camera>::add(this);
	}

	Camera::~Camera()
	{
		glDeleteFramebuffers(1, &fbo);
		Collector<Camera>::remove(this);
	}

	json Camera::serialize()
	{
		json j = Actor::serialize();
		j["typeID"] = TRISTEON_TYPENAME(Camera);

		j["screenSize"] = screenSize;
		j["screenCoordinates"] = screenCoordinates;
		j["renderToScreen"] = renderToScreen;
		j["overrideResolution"] = overrideResolution;

		j["zoom"] = zoom;
		return j;
	}

	void Camera::deserialize(json j)
	{
		Actor::deserialize(j);
		
		screenSize = j.value("screenSize", Vector2::one());
		screenCoordinates = j.value("screenCoordinates", Vector2 { -1 , -1 });
		renderToScreen = j.value("renderToScreen", true);
		overrideResolution = j.value("overrideResolution", Vector2Int{ 800, 800 });
		zoom = j.value("zoom", 1.0f);
	}

	void Camera::drawGizmos()
	{
		if (zoom == 0)
			return;

		Vector2 resolution = (Vector2)Window::gameSize() * screenSize;
		if (!renderToScreen)
			resolution = overrideResolution;
		
		Gizmos::drawSquare(position, resolution * (1.0f / zoom), 0, Colour(0.8, 0.8, 0.8, 0.5));
	}

	void Camera::buildFramebuffer()
	{
		lastScreenSize = screenSize;
		lastWindowSize = Window::gameSize();

		auto size = static_cast<Vector2Int>(lastScreenSize * lastWindowSize);

		//Delete old framebuffer with its texture
		if (fbo != NULL)
			glDeleteFramebuffers(1, &fbo);
		if (fboTexture != NULL)
			glDeleteTextures(1, &fboTexture);

		//Gen and bind Framebuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//Create texture
		glGenTextures(1, &fboTexture);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Finish
		isValid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		if (!isValid)
			Console::warning("Failed to create camera's framebuffer with size " + size.toString());
		else
			Console::write("Successfully created camera's framebuffer " + std::to_string(fbo));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Camera::bindFramebuffer()
	{
		if (!isValid || lastScreenSize != screenSize || lastWindowSize != Window::gameSize())
			buildFramebuffer();
		
		if (!isValid)
			return;
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void Camera::drawToScreen()
	{
		static Shader shader("Internal/Shaders/Camera.vert", "Internal/Shaders/Camera.frag");

		if (!isValid)
			return;

		if (Keyboard::pressed(Keyboard::R))
			shader.reload();

		shader.bind();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		shader.setUniformValue("screenTexture", 0);
		shader.setUniformValue("screenSize", screenSize.x, screenSize.y);
		shader.setUniformValue("screenCoordinates", screenCoordinates.x, screenCoordinates.y);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}