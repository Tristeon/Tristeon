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
		glDeleteFramebuffers(1, &_fbo);
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
		_lastScreenSize = screenSize;
		_lastWindowSize = Window::gameSize();

		const auto size = static_cast<Vector2Int>(_lastScreenSize * _lastWindowSize);

		//Delete old framebuffer with its texture
		if (_fbo != NULL)
			glDeleteFramebuffers(1, &_fbo);
		if (_fboTexture != NULL)
			glDeleteTextures(1, &_fboTexture);

		//Gen and bind Framebuffer
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		//Create texture
		glGenTextures(1, &_fboTexture);
		glBindTexture(GL_TEXTURE_2D, _fboTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Finish
		_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		if (!_valid)
			Console::warning("Failed to create camera's framebuffer with size " + size.toString());
		else
			Console::write("Successfully created camera's framebuffer " + std::to_string(_fbo));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Camera::bindFramebuffer()
	{
		if (_lastScreenSize != screenSize || _lastWindowSize != Window::gameSize())
			buildFramebuffer();
		
		if (!_valid)
			return;
		
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void Camera::drawToScreen() const
	{
		static Shader shader("Internal/Shaders/Camera.vert", "Internal/Shaders/Camera.frag");

		if (!_valid)
			return;

		shader.bind();

		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _fboTexture);
		shader.setUniformValue("screenTexture", 0);

		//Pass screen info
		shader.setUniformValue("screenSize", screenSize.x, screenSize.y);
		shader.setUniformValue("screenCoordinates", screenCoordinates.x, screenCoordinates.y);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}