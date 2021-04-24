#include "Camera.h"

#include <glad/glad.h>


#include "DebugGL.h"
#include "Window.h"
#include "PostProcessing/PostProcessingEffect.h"
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
		TRISTEON_DEBUG_GL();
		glDeleteTextures(1, &_fboTexture);
		TRISTEON_DEBUG_GL();
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
		const auto r = resolution();
		Gizmos::drawSquare(position, (Vector)r * (1.0f / zoom), 0, Colour(0.8, 0.8, 0.8, 0.5));
	}

	VectorU Camera::resolution()
	{
		Vector resolution = ((Vector)Window::gameSize() * screenSize);
		if (!renderToScreen)
			resolution = overrideResolution;
		return (VectorU)resolution;
	}

	void Camera::buildFramebuffers()
	{
		_lastScreenSize = screenSize;
		_lastWindowSize = Window::gameSize();

		createFramebuffer();
		createProcessingFramebuffers();
		createCompositeLightBuffer();
	}

	void Camera::createFramebuffer()
	{
		const VectorU size = resolution();

		//Delete old framebuffer with its texture
		if (_fbo != NULL)
			glDeleteFramebuffers(1, &_fbo);
		if (_fboTexture != NULL)
			glDeleteTextures(1, &_fboTexture);

		//Gen and bind Framebuffer
		glGenFramebuffers(1, &_fbo);
		TRISTEON_DEBUG_GL();
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		TRISTEON_DEBUG_GL();

		//Create texture
		glGenTextures(1, &_fboTexture);
		TRISTEON_DEBUG_GL();
		glBindTexture(GL_TEXTURE_2D, _fboTexture);
		TRISTEON_DEBUG_GL();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		TRISTEON_DEBUG_GL();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		TRISTEON_DEBUG_GL();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTexture, 0);
		TRISTEON_DEBUG_GL();
		glBindTexture(GL_TEXTURE_2D, 0);
		TRISTEON_DEBUG_GL();

		//Finish
		_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		if (!_valid)
			TRISTEON_WARNING("Failed to create camera's framebuffer with size " + size.toString());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		TRISTEON_DEBUG_GL();
	}

	void Camera::createProcessingFramebuffers()
	{
		const VectorU size = resolution();
		{
			//Delete old framebuffer with its texture
			if (_processedFbo != NULL)
				glDeleteFramebuffers(1, &_processedFbo);
			if (_processedFboTexture != NULL)
				glDeleteTextures(1, &_processedFboTexture);

			//Gen and bind Framebuffer
			glGenFramebuffers(1, &_processedFbo);
			TRISTEON_DEBUG_GL();
			glBindFramebuffer(GL_FRAMEBUFFER, _processedFbo);
			TRISTEON_DEBUG_GL();

			//Create texture
			glGenTextures(1, &_processedFboTexture);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, _processedFboTexture);
			TRISTEON_DEBUG_GL();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			TRISTEON_DEBUG_GL();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _processedFboTexture, 0);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, 0);
			TRISTEON_DEBUG_GL();

			//Finish
			_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			if (!_valid)
				TRISTEON_WARNING("Failed to create camera's framebuffer with size " + size.toString());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			TRISTEON_DEBUG_GL();
		}
		{
			//Delete old framebuffer with its texture
			if (_previousProcessFbo != NULL)
				glDeleteFramebuffers(1, &_previousProcessFbo);
			if (_previousProcessFboTexture != NULL)
				glDeleteTextures(1, &_previousProcessFboTexture);

			//Gen and bind Framebuffer
			glGenFramebuffers(1, &_previousProcessFbo);
			TRISTEON_DEBUG_GL();
			glBindFramebuffer(GL_FRAMEBUFFER, _previousProcessFbo);
			TRISTEON_DEBUG_GL();

			//Create texture
			glGenTextures(1, &_previousProcessFboTexture);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, _previousProcessFboTexture);
			TRISTEON_DEBUG_GL();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			TRISTEON_DEBUG_GL();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _previousProcessFboTexture, 0);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, 0);
			TRISTEON_DEBUG_GL();

			//Finish
			_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			if (!_valid)
				TRISTEON_WARNING("Failed to create camera's framebuffer with size " + size.toString());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			TRISTEON_DEBUG_GL();
		}
	}

	void Camera::createCompositeLightBuffer()
	{
		const auto size = resolution();

		//Delete old framebuffer with its texture
		if (_compositeLightFBOs[0] != NULL)
			glDeleteFramebuffers((int)_compositeLightFBOs.size(), _compositeLightFBOs.data());
		if (_compositeLightTextures[0] != NULL)
			glDeleteTextures((int)_compositeLightTextures.size(), _compositeLightTextures.data());

		//Gen and bind Framebuffer
		glGenFramebuffers((int)_compositeLightFBOs.size(), _compositeLightFBOs.data());
		TRISTEON_DEBUG_GL();
		glGenTextures((int)_compositeLightTextures.size(), _compositeLightTextures.data());
		TRISTEON_DEBUG_GL();
		for (auto i = 0; i < (int)_compositeLightFBOs.size(); i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _compositeLightFBOs[i]);
			TRISTEON_DEBUG_GL();

			//Create texture
			glBindTexture(GL_TEXTURE_2D, _compositeLightTextures[i]);
			TRISTEON_DEBUG_GL();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			TRISTEON_DEBUG_GL();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			TRISTEON_DEBUG_GL();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _compositeLightTextures[i], 0);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, 0);
			TRISTEON_DEBUG_GL();
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			TRISTEON_DEBUG_GL();

			//Finish
			_valid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
			if (!_valid)
				TRISTEON_WARNING("Failed to create composite light framebuffer with size " + size.toString());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			TRISTEON_DEBUG_GL();
		}
	}

	void Camera::updateFramebuffers()
	{
		if (_lastScreenSize != screenSize || _lastWindowSize != Window::gameSize())
			buildFramebuffers();
	}

	void Camera::applyPostProcessing()
	{
		const auto effects = findBehaviours<PostProcessingEffect>();
		const auto res = resolution();
		
		if (effects.empty())
		{
			//Copy directly
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
			TRISTEON_DEBUG_GL();
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _processedFbo);
			TRISTEON_DEBUG_GL();
			glBlitFramebuffer(0, 0, res.x, res.y, 0, 0, res.x, res.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			TRISTEON_DEBUG_GL();
			return;
		}

		//Set default previous
		glBindFramebuffer(GL_FRAMEBUFFER, _previousProcessFbo);
		TRISTEON_DEBUG_GL();
		glClear(GL_COLOR_BUFFER_BIT);
		TRISTEON_DEBUG_GL();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
		TRISTEON_DEBUG_GL();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _previousProcessFbo);
		TRISTEON_DEBUG_GL();
		glBlitFramebuffer(0, 0, res.x, res.y, 0, 0, res.x, res.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		TRISTEON_DEBUG_GL();

		//Draw effects
		for (auto* effect : effects)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _processedFbo);
			TRISTEON_DEBUG_GL();
			glViewport(0, 0, res.x, res.y);
			TRISTEON_DEBUG_GL();
			glClear(GL_COLOR_BUFFER_BIT);
			TRISTEON_DEBUG_GL();

			effect->shader()->bind();
			effect->shader()->setUniformValue("original", 0);
			effect->shader()->setUniformValue("previous", 1);
			
			glActiveTexture(GL_TEXTURE0);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, _fboTexture);
			TRISTEON_DEBUG_GL();

			glActiveTexture(GL_TEXTURE1);
			TRISTEON_DEBUG_GL();
			glBindTexture(GL_TEXTURE_2D, _previousProcessFboTexture);
			TRISTEON_DEBUG_GL();

			effect->render();
			
			//Copy result into separate texture
			glBindFramebuffer(GL_FRAMEBUFFER, _previousProcessFbo);
			TRISTEON_DEBUG_GL();
			glClear(GL_COLOR_BUFFER_BIT);
			TRISTEON_DEBUG_GL();
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _processedFbo);
			TRISTEON_DEBUG_GL();
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _previousProcessFbo);
			TRISTEON_DEBUG_GL();
			glBlitFramebuffer(0, 0, res.x, res.y, 0, 0, res.x, res.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			TRISTEON_DEBUG_GL();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}
	}

	void Camera::drawToScreen() const
	{
		static Shader shader("Internal/Shaders/Camera.vert", "Internal/Shaders/Texture.frag");

		if (!_valid)
			return;

		shader.bind();

		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		TRISTEON_DEBUG_GL();
		glBindTexture(GL_TEXTURE_2D, _processedFboTexture);
		TRISTEON_DEBUG_GL();
		shader.setUniformValue("texture", 0);

		//Pass screen info
		shader.setUniformValue("screenSize", screenSize.x, screenSize.y);
		shader.setUniformValue("screenCoordinates", screenCoordinates.x, screenCoordinates.y);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		TRISTEON_DEBUG_GL();
		glBindTexture(GL_TEXTURE_2D, 0);
		TRISTEON_DEBUG_GL();
	}
}