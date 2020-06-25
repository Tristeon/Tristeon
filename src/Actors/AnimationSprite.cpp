#include "AnimationSprite.h"

#include <Animations/AnimationClip.h>

#include "AssetDatabase.h"
#include "Math/Math.h"
#include "Resources.h"
#include "Utils/Time.h"

namespace Tristeon
{
	REGISTER_ACTOR_CPP(AnimationSprite)
	REGISTER_TYPE_CPP(AnimationSprite)

	json AnimationSprite::serialize()
	{
		json j = Sprite::serialize();
		j["typeID"] = TRISTEON_TYPENAME(AnimationSprite);
		j["clipPath"] = clipPath;
		return j;
	}

	void AnimationSprite::deserialize(json j)
	{
		Sprite::deserialize(j);
		setAnimationClip(j.value("clipPath", ""));
	}

	void AnimationSprite::setPaused(bool const& value)
	{
		paused = value;
	}

	void AnimationSprite::setAnimationClip(String const& clipPath)
	{
		if (clip != nullptr && clipPath == this->clipPath)
			return;

		auto* clip = Resources::jsonLoad<AnimationClip>(clipPath);
		if (clip != nullptr)
		{
			this->clip = clip;
			this->clipPath = clipPath;
			
			setTexture(clip->texturePath, false);
			currentFrame = 0;
		}
	}

	void AnimationSprite::setFrame(unsigned int const& frame)
	{
		currentFrame = Math::clamp(frame, clip->startIndex, clip->endIndex);
	}

	void AnimationSprite::render()
	{
		auto* shader = getShader();
		if (clip != nullptr)
		{
			shader->setUniformValue("animation.frame", static_cast<unsigned int>(floor(currentFrame)) + clip->startIndex);
			shader->setUniformValue("animation.cols", clip->cols);
			shader->setUniformValue("animation.rows", clip->rows);
			
			shader->setUniformValue("spacing.left", clip->spacing.left);
			shader->setUniformValue("spacing.right", clip->spacing.right);
			shader->setUniformValue("spacing.top", clip->spacing.top);
			shader->setUniformValue("spacing.bottom", clip->spacing.bottom);
			shader->setUniformValue("spacing.horizontalFrame", clip->spacing.horizontalFrame);
			shader->setUniformValue("spacing.verticalFrame", clip->spacing.verticalFrame);
		}
		Sprite::render();
	}

	void AnimationSprite::update()
	{
		if (clip == nullptr)
			return;

		if (paused)
			return;

		if (clip->startIndex + floor(currentFrame) >= clip->endIndex)
		{
			if (clip->loops)
				currentFrame = 0;
			else
				return; //Simply hold onto the last frame if we aren't looping
		}
		std::cout << "Current frame: " << currentFrame << ". Playbackrate: " << clip->playbackRate << std::endl;
		currentFrame += 0.01f * clip->playbackRate * Time::deltaTime();
	}

	Shader* AnimationSprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/AnimatedSprite.frag");
		return &shader;
	}
}
