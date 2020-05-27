#include "AnimationSprite.h"

#include <QOpenGLShaderProgram>
#include <Animations/AnimationClip.h>

#include "AssetDatabase.h"
#include "Math/Math.h"
#include "Rendering/GameView.h"
#include "Resources.h"

namespace Tristeon
{
	REGISTER_ACTOR_CPP(AnimationSprite)
	REGISTER_TYPE_CPP(AnimationSprite)

	json AnimationSprite::serialize()
	{
		json j = Sprite::serialize();
		j["typeID"] = TRISTEON_TYPENAME(AnimationSprite);
		j["clipPath"] = clip ? clip->filePath : "";
		return j;
	}

	void AnimationSprite::deserialize(json j)
	{
		Sprite::deserialize(j);
		setAnimationClip(j["clipPath"]);
	}

	void AnimationSprite::setPaused(bool const& value)
	{
		paused = value;
	}

	void AnimationSprite::setAnimationClip(String const& clipPath)
	{
		if (clip != nullptr && clipPath == clip->filePath)
			return;
		
		this->clip = Resources::jsonLoad<AnimationClip>(clipPath);
		if (clip != nullptr)
		{
			clip->filePath = clipPath;
			setTexture(clip->texturePath, false);
			currentFrame = 0;
		}
	}

	void AnimationSprite::setFrame(unsigned int const& frame)
	{
		currentFrame = Math::clamp(frame, clip->startIndex, clip->endIndex);
	}

	void AnimationSprite::render(QOpenGLShaderProgram* program)
	{
		if (clip != nullptr)
		{
			program->setUniformValue("animation.frame", static_cast<int>(floor(currentFrame)) + clip->startIndex);
			program->setUniformValue("animation.cols", clip->cols);
			program->setUniformValue("animation.rows", clip->rows);

			program->setUniformValue("spacing.left", clip->spacing.left);
			program->setUniformValue("spacing.right", clip->spacing.right);
			program->setUniformValue("spacing.top", clip->spacing.top);
			program->setUniformValue("spacing.bottom", clip->spacing.bottom);
			program->setUniformValue("spacing.horizontalFrame", clip->spacing.horizontalFrame);
			program->setUniformValue("spacing.verticalFrame", clip->spacing.verticalFrame);
		}

		Sprite::render(program);
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
		currentFrame += 0.01f * clip->playbackRate * GameView::deltaTime();
	}

	Shader* AnimationSprite::getShader()
	{
		static Shader shader = Shader("Internal/Shaders/Sprite.vert", "Internal/Shaders/AnimatedSprite.frag");
		return &shader;
	}
}
