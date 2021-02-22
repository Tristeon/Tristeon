#include "AnimationSprite.h"

#include <Animations/AnimationClip.h>

#include "AssetManagement/AssetDatabase.h"
#include "AssetManagement/Resources.h"
#include "Math/Math.h"
#include "Utils/Time.h"

namespace Tristeon
{
	json AnimationSprite::serialize()
	{
		json j = Sprite::serialize();
		j["typeID"] = Type<AnimationSprite>::fullName();
		j["clipPath"] = _clipPath;
		return j;
	}

	void AnimationSprite::deserialize(json j)
	{
		Sprite::deserialize(j);
		setAnimationClip(j.value("clipPath", ""));
	}

	void AnimationSprite::setPaused(bool const& value)
	{
		_paused = value;
	}

	void AnimationSprite::setAnimationClip(String const& clipPath)
	{
		//Don't reapply the same clip
		if (_clip != nullptr && clipPath == this->_clipPath)
			return;

		//Attempt to load the new clip
		auto* clip = Resources::jsonLoad<AnimationClip>(clipPath);
		if (clip != nullptr)
		{
			this->_clip = clip;
			this->_clipPath = clipPath;
			
			setTexture(clip->albedoPath, false);
			setTexture(clip->normalPath, false, TextureType::Normal);
			setTexture(clip->lightMaskPath, false, TextureType::LightMask);
			_normalMapStrength = clip->normalMapStrength;
			_currentFrame = 0;
		}
	}

	void AnimationSprite::setFrame(unsigned int const& frame)
	{
		_currentFrame = (float)Math::clamp((int)frame, (int)_clip->startIndex, (int)_clip->endIndex);
	}

	void AnimationSprite::render(const float& depth)
	{
		auto* shader = getShader();
		shader->bind();
		if (_clip != nullptr)
		{
			shader->setUniformValue("animation.frame", static_cast<unsigned int>(floor(_currentFrame)) + _clip->startIndex);
			shader->setUniformValue("animation.cols", _clip->cols);
			shader->setUniformValue("animation.rows", _clip->rows);
			
			shader->setUniformValue("spacing.left", _clip->spacing.left);
			shader->setUniformValue("spacing.right", _clip->spacing.right);
			shader->setUniformValue("spacing.top", _clip->spacing.top);
			shader->setUniformValue("spacing.bottom", _clip->spacing.bottom);
			shader->setUniformValue("spacing.horizontalFrame", _clip->spacing.horizontalFrame);
			shader->setUniformValue("spacing.verticalFrame", _clip->spacing.verticalFrame);
		}
		Sprite::render(depth);
	}

	void AnimationSprite::update()
	{
		if (_clip == nullptr)
			return;

		if (_paused)
			return;

		if (_clip->startIndex + (unsigned int)floor(_currentFrame) >= _clip->endIndex)
		{
			if (_clip->loops)
				_currentFrame = 0;
			else
				return; //Simply hold onto the last frame if we aren't looping
		}
		_currentFrame += 0.01f * _clip->playbackRate * Time::deltaTime();
	}

	Shader* AnimationSprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/AnimatedSprite.frag");
		return &shader;
	}
}