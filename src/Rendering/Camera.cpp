#include "Camera.h"

#include <glad/glad.h>

#include "Window.h"
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
		j["typeID"] = Type<Camera>::fullName();

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
		
		screenSize = j.value("screenSize", Vector::one());
		screenCoordinates = j.value("screenCoordinates", Vector { -1 , -1 });
		renderToScreen = j.value("renderToScreen", true);
		overrideResolution = j.value("overrideResolution", VectorI{ 800, 800 });
		zoom = j.value("zoom", 1.0f);
	}

	void Camera::drawGizmos()
	{
		if (zoom == 0)
			return;

		Vector resolution = (Vector)Window::gameSize() * screenSize;
		if (!renderToScreen)
			resolution = overrideResolution;
		
		Gizmos::drawSquare(position, resolution * (1.0f / zoom), 0, Colour(0.8, 0.8, 0.8, 0.5));
	}

	Framebuffer Camera::framebuffer()
	{
		updateFramebuffer();
		
		VectorI resolution = (VectorI)((Vector)Window::gameSize() * screenSize);
		if (!renderToScreen)
			resolution = overrideResolution;
		
		return {
			_fbo,
			{
				0u,
				0u,
				(unsigned int)resolution.x,
				(unsigned int)resolution.y
			}
		};
	}

	void Camera::buildFramebuffer()
	{
		_lastScreenSize = screenSize;
		_lastWindowSize = Window::gameSize();

		const auto size = static_cast<VectorI>(_lastScreenSize * _lastWindowSize);

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
			TRISTEON_WARNING("Failed to create camera's framebuffer with size " + size.toString());
		else
			TRISTEON_LOG("Successfully created camera's framebuffer " + std::to_string(_fbo));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Camera::updateFramebuffer()
	{
		if (_lastScreenSize != screenSize || _lastWindowSize != Window::gameSize())
			buildFramebuffer();
	}
	
	void Camera::drawToScreen() const
	{
		static Shader shader("Internal/Shaders/Camera.vert", "Internal/Shaders/Texture.frag");

		if (!_valid)
			return;

		shader.bind();

		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _fboTexture);
		shader.setUniformValue("texture", 0);

		//Pass screen info
		shader.setUniformValue("screenSize", screenSize.x, screenSize.y);
		shader.setUniformValue("screenCoordinates", screenCoordinates.x, screenCoordinates.y);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}