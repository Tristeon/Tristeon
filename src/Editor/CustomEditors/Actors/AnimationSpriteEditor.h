#pragma once
#ifdef TRISTEON_EDITOR
#include "Actors/AnimationSprite.h"
#include "SpriteEditor.h"

namespace TristeonEditor
{
	class AnimationSpriteEditor : public SpriteEditor
	{
		CUSTOM_EDITOR_H(Tristeon::AnimationSprite, AnimationSpriteEditor)
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	protected:
		void displayProperties() override;

		Tristeon::AnimationSprite* animSprite = nullptr;
	};
}
#endif