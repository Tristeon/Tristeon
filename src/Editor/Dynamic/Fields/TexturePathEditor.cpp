#include "TexturePathEditor.h"

#include <qboxlayout.h>
#include <qdir.h>
#include <qfiledialog.h>

#include <Settings.h>
#include <AssetManagement/AssetDatabase.h>

namespace TristeonEditor
{
	TexturePathEditor::TexturePathEditor(const nlohmann::json& pValue, const std::function<void(nlohmann::json)>& pCallback): AbstractJsonEditor(pValue, pCallback)
	{
		std::string path = _value["path"];
		_button = new QPushButton(path.c_str());
		_widget = _button;
		QWidget::connect(_button, &QPushButton::clicked, 
			[=]()
			{
				QDir const baseDir(Tristeon::Settings::assetPath().c_str());

				auto const path = QFileDialog::getOpenFileName(nullptr, QWidget::tr("Find Texture"), Tristeon::Settings::assetPath().c_str(), QWidget::tr("Image Files (*.png *.jpg *.bmp)"));
				auto const localPath = baseDir.relativeFilePath(path);
				auto const fileName = QFileInfo(path).baseName();

				_value["guid"] = Tristeon::AssetDatabase::guid("assets://" + localPath.toStdString());
				_button->setText(fileName);
				_callback(_value);
			});
	}

	void TexturePathEditor::setValue(const nlohmann::json& pValue)
	{
		_value = pValue;
		_button->setText(Tristeon::AssetDatabase::path(pValue.value("guid", 0)).c_str());
	}
}
