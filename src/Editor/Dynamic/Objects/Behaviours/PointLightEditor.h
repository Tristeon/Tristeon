#pragma once
#ifdef TRISTEON_EDITOR
#include "LightEditor.h"
#include <Rendering/Lighting/PointLight.h>
#include <Editor/Dynamic/Objects/ObjectEditorRegister.h>

namespace TristeonEditor
{
	class PointLightEditor : public LightEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::PointLight* _point = nullptr;
	};
	
	OBJECT_EDITOR(Tristeon::PointLight, PointLightEditor);
}
#endif
