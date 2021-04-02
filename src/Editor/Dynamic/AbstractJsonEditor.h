#pragma once
#include "AbstractEditor.h"

#include <Utils/ClassDefaults.h>
#include <json.h>
#include <qwidget.h>

namespace TristeonEditor
{
	class AbstractJsonEditor : public AbstractEditor
	{
	public:
		AbstractJsonEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback) :
			_value(pValue), _callback(pCallback) { }
		virtual ~AbstractJsonEditor() = default;
		virtual nlohmann::json value() { return _value; }
		virtual void setValue(const nlohmann::json& pValue) = 0;
		virtual QWidget* widget() override = 0;

		DELETE_COPY(AbstractJsonEditor);
		DEFAULT_MOVE(AbstractJsonEditor);

	protected:
		nlohmann::json _value;
		std::function<void(nlohmann::json)> _callback = nullptr;
	};
}