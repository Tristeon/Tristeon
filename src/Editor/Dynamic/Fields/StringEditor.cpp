#include "StringEditor.h"

namespace TristeonEditor
{
	StringEditor::StringEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		_lineEdit = new QLineEdit();
		_widget = _lineEdit;
		_lineEdit->setText(QString::fromStdString(pValue));
		QWidget::connect(_lineEdit, &QLineEdit::textChanged, [=](const QString& val) { _callback(val.toStdString()); });
	}

	void StringEditor::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;
		_lineEdit->setText(pValue.get<std::string>().c_str());
	}
}