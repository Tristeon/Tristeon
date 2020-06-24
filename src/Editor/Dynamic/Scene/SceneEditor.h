#pragma once
#ifdef TRISTEON_EDITOR
#include "Editor/Dynamic/AbstractEditor.h"
#include "Scenes/Layers/Layer.h"

namespace TristeonEditor
{
	class SceneEditor : public AbstractEditor
	{
	public:
		virtual void updateView() = 0;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	protected:
		Tristeon::Layer* currentLayer = nullptr;
	};
}
#endif