#ifdef TRISTEON_EDITOR
#include "AnimationSpriteEditor.h"

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::AnimationSprite, AnimationSpriteEditor)

	void AnimationSpriteEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		SpriteEditor::targetChanged(current, old);
		animSprite = (Tristeon::AnimationSprite*)current;
	}

	void AnimationSpriteEditor::displayProperties()
	{
		SpriteEditor::displayProperties();

		//QWidget* formWidget = new QWidget(this);
		//QFormLayout* form = new QFormLayout(formWidget);
		//formWidget->setLayout(form);
	}
}
#endif