#pragma once
#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include "Physics/CircleCollider.h"

namespace TristeonEditor
{
	class CircleColliderEditor : public BehaviourEditor
	{
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::CircleCollider* circle = nullptr;
	};
	OBJECT_EDITOR(Tristeon::CircleCollider, CircleColliderEditor);
}
#endif