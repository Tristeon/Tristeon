#ifdef TRISTEON_EDITOR
#include "JsonFileEditor.h"
#include "Serialization/JsonSerializer.h"
#include "Editor/EditorFields.h"
#include "Math/Vector2Int.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace TristeonEditor
{
	FILE_EDITOR_CPP("json", JsonFileEditor)

	void JsonFileEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		FileEditor::targetChanged(current, old);
		data = Tristeon::JsonSerializer::load(item->path);
	}

	void JsonFileEditor::initialize()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		displayJson(data, formWidget, form);
	}

	void JsonFileEditor::displayJson(json& data, QWidget * parent, QFormLayout * form)
	{
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (!data.is_object() || it.key() == "typeID") continue;

			switch (it.value().type())
			{
				case detail::value_t::boolean:
				{
					EditorFields::boolField(form, it.key(), data[it.key()], [&](int state) { data[it.key()] = (bool)((Qt::CheckState)state == Qt::Checked); saveData(); });
					break;
				}
				case detail::value_t::string:
				{
					EditorFields::stringField(form, it.key(), data[it.key()], [&](std::string value) { data[it.key()] = value; saveData(); });
					break;
				}
				case detail::value_t::object:
				{
					Tristeon::String const type = it.value().contains("typeID") ? it.value()["typeID"] : "";

					bool const isVector2 = type == TRISTEON_TYPENAME(Tristeon::Vector2);
					bool const isVector2Int = type == TRISTEON_TYPENAME(Tristeon::Vector2Int);
					bool const isVector3 = type == TRISTEON_TYPENAME(Tristeon::Vector3);
					bool const isVector4 = type == TRISTEON_TYPENAME(Tristeon::Vector4);

					QWidget* field;
					if (isVector2 || isVector2Int || isVector3 || isVector4)
					{
						field = new QWidget(parent);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						if (isVector2 || isVector3 || isVector4)
						{
							auto* x = EditorFields::floatField(field, it.value()["x"], [&](float value) { data[it.key()]["x"] = value; saveData(); });
							auto* y = EditorFields::floatField(field, it.value()["y"], [&](float value) { data[it.key()]["y"] = value; saveData(); });
							layout->addWidget(x);
							layout->addWidget(y);
						}
						if (isVector3 || isVector4)
						{
							auto* z = EditorFields::floatField(field, it.value()["z"], [&](float value) { data[it.key()]["z"] = value; saveData(); });
							layout->addWidget(z);
						}
						if (isVector4)
						{
							auto* w = EditorFields::floatField(field, it.value()["w"], [&](float value) { data[it.key()]["w"] = value; saveData(); });
							layout->addWidget(w);
						}
						if (isVector2Int)
						{
							auto* x = EditorFields::intField(field, it.value()["x"], [&](int value) { data[it.key()]["x"] = value; saveData(); });
							auto* y = EditorFields::intField(field, it.value()["y"], [&](int value) { data[it.key()]["y"] = value; saveData(); });
							layout->addWidget(x);
							layout->addWidget(y);
						}
					}
					else
					{
						field = new QLabel("Nested objects aren't supported yet");
					}

					if (field != nullptr)
						form->addRow(new QLabel(QString::fromStdString(it.key())), field);
					break;
				}
				case detail::value_t::array:
				{
					//TODO: Support arrays in json file editor
					form->addRow(new QLabel(QString::fromStdString(it.key())), new QLabel("Arrays are not default supported in the editor"));
					break;
				}
				case detail::value_t::number_float:
				{
					EditorFields::floatField(form, it.key(), it.value(), [&](float value) { data[it.key()] = value; saveData(); });
					break;
				}
				case detail::value_t::number_integer:
				{
					EditorFields::intField(form, it.key(), it.value(), [&](int value) { data[it.key()] = value; saveData(); });
					break;
				}
				case detail::value_t::number_unsigned:
				{
					EditorFields::uintField(parent, it.value(), 0, std::numeric_limits<int>::max(), [&](int value){ data[it.key()] = (unsigned int)value; saveData(); });
					break;
				}
				default:;
			}
		}

	}

	void JsonFileEditor::saveData()
	{
		Tristeon::JsonSerializer::save(item->path, data);
	}
}
#endif