#pragma once
#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include "Physics/CircleCollider.h"

namespace TristeonEditor
{
	class CircleColliderEditor : public BehaviourEditor
	{
		OBJECT_EDITOR_H(Tristeon::CircleCollider, CircleColliderEditor);
	public:
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
		void displayContents() override;
	private:
		Tristeon::CircleCollider* circle = nullptr;
	};
}
#endif