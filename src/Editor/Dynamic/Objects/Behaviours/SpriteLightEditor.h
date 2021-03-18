#pragma once
#include <qpushbutton.h>

#include "LightEditor.h"
#include "Rendering/Lighting/SpriteLight.h"

namespace TristeonEditor
{
	class SpriteLightEditor : public LightEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::SpriteLight* _spriteLight = nullptr;
		void changeTexturePressed(QPushButton* pButton);
	};
	OBJECT_EDITOR(Tristeon::SpriteLight, SpriteLightEditor);
}
