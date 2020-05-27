#pragma once
#ifdef TRISTEON_EDITOR
#include "Actors/AnimationSprite.h"
#include "SpriteEditor.h"

namespace TristeonEditor
{
	class AnimationSpriteEditor : public SpriteEditor
	{
		OBJECT_EDITOR_H(Tristeon::AnimationSprite, AnimationSpriteEditor);
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		bool shouldDisplay(Tristeon::String const& propertyName) override;
	protected:
		void textureButton() override;
		void loadAnimationClip();

		Tristeon::AnimationSprite* animSprite = nullptr;
	};
}
#endif