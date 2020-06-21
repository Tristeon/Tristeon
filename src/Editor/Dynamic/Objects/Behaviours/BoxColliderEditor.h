#pragma once
#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include "Physics/BoxCollider.h"

namespace TristeonEditor
{
	class BoxColliderEditor : public BehaviourEditor
	{
		OBJECT_EDITOR_H(Tristeon::BoxCollider, BoxColliderEditor);
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::BoxCollider* box = nullptr;
	};
}
#endif