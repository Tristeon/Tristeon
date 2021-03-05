#pragma once
#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include <Rendering/Lighting/Light.h>

namespace TristeonEditor
{
	class LightEditor : public BehaviourEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::Light* light = nullptr;
	};
	OBJECT_EDITOR(Tristeon::Light, LightEditor);
}

#endif