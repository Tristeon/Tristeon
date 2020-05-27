#pragma once
#ifdef TRISTEON_EDITOR
#include <QtWidgets>
#include "ActorEditor.h"
#include <Actors/Sprite.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>

namespace TristeonEditor
{
	class SpriteEditor : public ActorEditor
	{
		OBJECT_EDITOR_H(Tristeon::Sprite, SpriteEditor);
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayProperties() override;
		bool shouldDisplay(Tristeon::String const& propertyName) override;

	protected:
		virtual void textureButton();
		void changeTexturePressed();

		Tristeon::Sprite* sprite = nullptr;

		QPushButton* colorButton = nullptr;
	};
}
#endif