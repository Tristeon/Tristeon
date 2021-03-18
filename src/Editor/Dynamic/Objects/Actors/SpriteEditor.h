#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "ActorEditor.h"
#include <Rendering/Sprite.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>

namespace TristeonEditor
{
	class SpriteEditor : public ActorEditor
	{
	public:
		void targetChanged(Tristeon::TObject* pCurrent, Tristeon::TObject* pOld) override;
		void displayProperties() override;
		bool shouldDisplay(Tristeon::String const& pPropertyName) override;

	protected:
		virtual void displayTextureButtons();
		void changeTexturePressed(const Tristeon::TextureType& pType, QPushButton* pButton);

		Tristeon::Sprite* _sprite = nullptr;
		QPushButton* _colorButton = nullptr;
	};
	OBJECT_EDITOR(Tristeon::Sprite, SpriteEditor);
}
#endif