#pragma once
#include "PointLightEditor.h"
#include "Rendering/Lighting/SpotLight.h"

namespace TristeonEditor
{
	class SpotLightEditor : public PointLightEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::SpotLight* _spot = nullptr;
	};

	OBJECT_EDITOR(Tristeon::SpotLight, SpotLightEditor);
}
