#pragma once
#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include "Physics/BoxCollider.h"

namespace TristeonEditor
{
	class BoxColliderEditor : public BehaviourEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::BoxCollider* box = nullptr;
	};

	OBJECT_EDITOR(Tristeon::BoxCollider, BoxColliderEditor);
}
#endif