#pragma once
#ifdef TRISTEON_EDITOR
#include "Rendering/AnimationSprite.h"
#include "SpriteEditor.h"

namespace TristeonEditor
{
	class AnimationSpriteEditor : public SpriteEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		bool shouldDisplay(Tristeon::String const& propertyName) override;
	protected:
		void displayTextureButtons() override;
		void loadAnimationClip();

		Tristeon::AnimationSprite* animSprite = nullptr;
	};
	OBJECT_EDITOR(Tristeon::AnimationSprite, AnimationSpriteEditor);
}
#endif