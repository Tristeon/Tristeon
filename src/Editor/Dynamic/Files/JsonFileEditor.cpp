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

	void JsonFileEditor::displayJson(json& object, QWidget * parent, QFormLayout * form)
	{
		for (auto it = object.begin(); it != object.end(); ++it)
		{
			if (!object.is_object() || it.key() == "typeID") continue;

			auto* name = new QLabel(QString::fromStdString(it.key()), parent);
			QWidget* field = nullptr;
			
			switch (it.value().type())
			{
				case detail::value_t::boolean:
				{
					auto* box = new QCheckBox(parent);
					field = box;
					box->setCheckState(it.value().get<bool>() ? Qt::Checked : Qt::Unchecked);
					connect(box, &QCheckBox::stateChanged, this, [&](int state)
						{
							object[it.key()] = (Qt::CheckState)state == Qt::Checked;
							saveData();
						});
					break;
				}
				case detail::value_t::string:
				{
					auto line = new QLineEdit(parent);
					field = line;
					line->setText(QString::fromStdString(it.value()));
					connect(line, &QLineEdit::textChanged, this, [&](const QString & value)
						{
							object[it.key()] = value.toStdString();
							saveData();
						});
					break;
				}
				case detail::value_t::object:
				{
					Tristeon::String const type = it.value().contains("typeID") ? it.value()["typeID"] : "";

					bool const isVector2 = type == TRISTEON_TYPENAME(Tristeon::Vector2);
					bool const isVector2Int = type == TRISTEON_TYPENAME(Tristeon::Vector2Int);
					bool const isVector3 = type == TRISTEON_TYPENAME(Tristeon::Vector3);
					bool const isVector4 = type == TRISTEON_TYPENAME(Tristeon::Vector4);

					if (isVector2 || isVector2Int || isVector3 || isVector4)
					{
						field = new QWidget(parent);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						if (isVector2 || isVector3 || isVector4)
						{
							auto* x = EditorFields::floatField(field, it.value()["x"], [&](float value)
								{
									object[it.key()]["x"] = value;
									saveData();
								});
							auto* y = EditorFields::floatField(field, it.value()["y"], [&](float value)
								{
									object[it.key()]["y"] = value;
									saveData();
								});

							layout->addWidget(x);
							layout->addWidget(y);
						}
						if (isVector3 || isVector4)
						{
							auto* z = EditorFields::floatField(field, it.value()["z"], [&](float value)
								{
									object[it.key()]["z"] = value;
									saveData();
								});
							layout->addWidget(z);
						}
						if (isVector4)
						{
							auto* w = EditorFields::floatField(field, it.value()["w"], [&](float value)
								{
									object[it.key()]["w"] = value;
									saveData();
								});
							layout->addWidget(w);
						}
					}
					else if (isVector2Int)
					{
						field = new QWidget(parent);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						auto* x = EditorFields::intField(field, it.value()["x"], [&](int value)
							{
								object[it.key()]["x"] = value;
								saveData();
							});
						auto* y = EditorFields::intField(field, it.value()["y"], [&](int value)
							{
								object[it.key()]["y"] = value;
								saveData();
							});

						layout->addWidget(x);
						layout->addWidget(y);
					}
					else
					{
						field = new QLabel("Nested objects aren't supported yet");
					}
					break;
				}
				case detail::value_t::array:
				{
					//TODO: Support arrays in json file editor
					field = new QLabel("Arrays not supported yet", parent);
					break;
				}
				case detail::value_t::number_float:
				{
					field = EditorFields::floatField(parent, it.value(), [&](float value)
						{
							object[it.key()] = value;
							saveData();
						});
					break;
				}
				case detail::value_t::number_integer:
				{
					field = EditorFields::intField(parent, it.value(), [&](int value)
						{
							object[it.key()] = value;
							saveData();
						});
					break;
				}
				case detail::value_t::number_unsigned:
				{
					field = EditorFields::intField(parent, it.value(), 0, std::numeric_limits<int>::max(), [&](int value)
						{
							object[it.key()] = (unsigned int)value;
							saveData();
						});
					break;
				}
				default:;
			}

			if (field != nullptr)
				form->addRow(name, field);
			else
			{
				name->deleteLater();
				name->hide();
			}
		}

	}

	void JsonFileEditor::saveData()
	{
		Tristeon::JsonSerializer::save(item->path, data);
	}
}
#endif