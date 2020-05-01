#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "ActorEditor.h"
#include <Actors/Sprite.h>
#include <Editor/CustomEditors/EditorRegister.h>

namespace TristeonEditor
{
	class SpriteEditor : public ActorEditor
	{
		CUSTOM_EDITOR_H(Tristeon::Sprite, SpriteEditor)

	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	protected:
		void displayProperties() override;
		virtual void textureButton();

		void changeTexturePressed();
		Tristeon::Sprite* sprite = nullptr;

		QPushButton* colorButton = nullptr;
	};
}
#endif