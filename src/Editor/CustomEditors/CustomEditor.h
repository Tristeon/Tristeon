#pragma once
#ifdef TRISTEON_EDITOR
#include <TObject.h>
#include <QWidget>
#include <QLayout>

namespace TristeonEditor
{
	class Editor;
	
	class CustomEditor : public QWidget
	{
	public:
		CustomEditor();
		virtual ~CustomEditor() = default;
		virtual void initialize() = 0;
		virtual void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) = 0;

		Tristeon::TObject* target() const;
		void target(Tristeon::TObject* value);

		Editor* editor() const;
		void editor(Editor* value);
		
	protected:
		QVBoxLayout* layout = nullptr;

	private:
		Editor* _editor = nullptr;
		Tristeon::TObject* _target = nullptr;
	};
}
#endif