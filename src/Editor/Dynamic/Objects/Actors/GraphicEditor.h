#pragma once
#ifdef TRISTEON_EDITOR
#include "ActorEditor.h"
#include "Rendering/Graphic.h"

namespace TristeonEditor
{
	class GraphicEditor : public ActorEditor
	{
	public:
		void targetChanged(Tristeon::TObject* pCurrent, Tristeon::TObject* pOld) override;
		void displayProperties() override;
	private:
		Tristeon::Graphic* _graphic = nullptr;
	};
}
#endif