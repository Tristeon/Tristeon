#pragma once
#ifdef TRISTEON_EDITOR
#include "TObject.h"
#include <QWidget>

namespace TristeonEditor
{
	class Editor;

	class AbstractEditor : public QWidget
	{
		friend class PropertyWindow;
	public:
		virtual ~AbstractEditor() = default;

		virtual void initialize() = 0;
		virtual void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) = 0;

		Tristeon::TObject* target() const { return _target; }
		void target(Tristeon::TObject* value) { Tristeon::TObject* old = _target; _target = value; targetChanged(_target, old); }
	private:
		Tristeon::TObject* _target = nullptr;
		Editor* _editor = nullptr;
	};
}
#endif