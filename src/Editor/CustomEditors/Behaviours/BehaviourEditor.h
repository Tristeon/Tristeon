#pragma once
#include <qspinbox.h>
#ifdef TRISTEON_EDITOR
#include <Editor/CustomEditors/CustomEditor.h>
#include <Editor/CustomEditors/EditorRegister.h>
#include <Actors/Behaviours/Behaviour.h>

namespace TristeonEditor
{
	class BehaviourEditor : public CustomEditor
	{
		CUSTOM_EDITOR_H(Tristeon::Behaviour, BehaviourEditor)
	public:
		void initialize() override;
		void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) override;
	protected:
		virtual void displayContents();
		void removeButtonPressed();

		QDoubleSpinBox* displayFloat(QWidget* parent, float value, std::function<void(float)> changeCallback);
		QDoubleSpinBox* displayFloat(QWidget* parent, float value, float minValue, float maxValue, std::function<void(float)> changeCallback);
		QSpinBox* displayInt(QWidget* parent, int value, std::function<void(int)> changeCallback);
		QSpinBox* displayInt(QWidget* parent, int value, int minValue, int maxValue, std::function<void(int)> changeCallback);
	private:
		Tristeon::Behaviour* behaviour = nullptr;
		json data;
	};
}
#endif