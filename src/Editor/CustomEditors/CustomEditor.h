#pragma once
#ifdef TRISTEON_EDITOR
#include <TObject.h>
#include <QWidget>

namespace TristeonEditor
{
	class CustomEditor : public QWidget
	{
	public:
		virtual void initialize() = 0;
		virtual void targetChanged(Tristeon::TObject* current, Tristeon::TObject* old) = 0;

		Tristeon::TObject* target() const;
		void target(Tristeon::TObject* value);

	private:
		Tristeon::TObject* _target = nullptr;
	};
}
#endif