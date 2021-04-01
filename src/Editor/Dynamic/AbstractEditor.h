#pragma once
#include <QFormLayout>

namespace TristeonEditor
{
	class AbstractEditor
	{
	public:
		AbstractEditor(QFormLayout* pForm, const Tristeon::String& pLabel, const json& pValue, const std::function<void(json)>& pCallback) :
			_form(pForm), _label(pLabel), _value(pValue), _callback(pCallback) { }
		virtual ~AbstractEditor() = default;
		virtual json value() = 0;
		virtual void setValue(const json& pValue) = 0;

		DELETE_COPY(AbstractEditor);
		DEFAULT_MOVE(AbstractEditor);
		
	protected:
		QFormLayout* _form = nullptr;
		Tristeon::String _label;
		json _value;
		std::function<void(json)> _callback = nullptr;
	};
}