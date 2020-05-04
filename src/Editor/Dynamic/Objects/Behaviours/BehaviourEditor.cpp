#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include "Editor/EditorFields.h"

#include <QtWidgets>
#include <Editor/Editor.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::Behaviour, BehaviourEditor);

	void BehaviourEditor::initialize()
	{
		data = behaviour->serialize();

		auto* titleBar = new QWidget(this);
		layout->addWidget(titleBar);

		auto* titleLayout = new QHBoxLayout(titleBar);
		titleLayout->setContentsMargins(0, 0, 0, 0);
		titleBar->setLayout(titleLayout);

		auto* typeName = new QLabel(QString::fromStdString(data["typeID"]), titleBar);
		typeName->setAlignment(Qt::AlignLeft);
		titleLayout->addWidget(typeName);

		auto* closeButton = new QToolButton(titleBar);
		closeButton->setIcon(QIcon(QPixmap(QString("Internal/Icons/cross.png"))));
		closeButton->setMaximumSize(15, 15);
		titleLayout->addWidget(closeButton);
		connect(closeButton, &QToolButton::clicked, this, &BehaviourEditor::removeButtonPressed);

		auto* line = new QFrame(this);
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		layout->addWidget(line);

		displayContents();
	}

	void BehaviourEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		behaviour = dynamic_cast<Tristeon::Behaviour*>(current);
	}

	void BehaviourEditor::displayContents()
	{
		auto* formWidget = new QWidget(this);
		layout->addWidget(formWidget);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);

		for (auto iterator = data.begin(); iterator != data.end(); ++iterator)
		{
			if (iterator.key() == "typeID") continue;

			auto * name = new QLabel(QString::fromStdString(iterator.key()), formWidget);
			QWidget * field = nullptr;

			switch (iterator.value().type())
			{
			case detail::value_t::boolean:
			{
				auto* box = new QCheckBox(formWidget);
				field = box;
				box->setCheckState(iterator.value().get<bool>() ? Qt::Checked : Qt::Unchecked);
				connect(box, &QCheckBox::stateChanged, this, [=](int state)
					{
						data[iterator.key()] = (Qt::CheckState)state == Qt::Checked;
						behaviour->deserialize(data);
					});
				break;
			}
			case detail::value_t::string:
			{
				auto line = new QLineEdit(formWidget);
				field = line;
				line->setText(QString::fromStdString(iterator.value()));
				connect(line, &QLineEdit::textChanged, this, [=](const QString & value)
					{
						data[iterator.key()] = value.toStdString();
						behaviour->deserialize(data);
					});
				break;
			}
			case detail::value_t::object:
			{
				if (iterator.value().contains("typeID"))
				{
					Tristeon::String type = iterator.value()["typeID"];
					if (type == TRISTEON_TYPENAME(Tristeon::Vector2))
					{
						field = new QWidget(formWidget);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						auto* x = EditorFields::floatField(field, iterator.value()["x"], [=](float value)
							{
								data[iterator.key()]["x"] = value;
								behaviour->deserialize(data);
							});
						auto* y = EditorFields::floatField(field, iterator.value()["y"], [=](float value)
							{
								data[iterator.key()]["y"] = value;
								behaviour->deserialize(data);
							});

						layout->addWidget(x);
						layout->addWidget(y);
					}
					else if (type == TRISTEON_TYPENAME(Tristeon::Vector3))
					{
						field = new QWidget(formWidget);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						auto* x = EditorFields::floatField(field, iterator.value()["x"], [=](float value)
							{
								data[iterator.key()]["x"] = value;
								behaviour->deserialize(data);
							});
						auto* y = EditorFields::floatField(field, iterator.value()["y"], [=](float value)
							{
								data[iterator.key()]["y"] = value;
								behaviour->deserialize(data);
							});
						auto* z = EditorFields::floatField(field, iterator.value()["z"], [=](float value)
							{
								data[iterator.key()]["z"] = value;
								behaviour->deserialize(data);
							});

						layout->addWidget(x);
						layout->addWidget(y);
						layout->addWidget(z);
					}
					else if (type == TRISTEON_TYPENAME(Tristeon::Vector4))
					{
						field = new QWidget(formWidget);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						auto* x = EditorFields::floatField(field, iterator.value()["x"], [=](float value)
							{
								data[iterator.key()]["x"] = value;
								behaviour->deserialize(data);
							});
						auto* y = EditorFields::floatField(field, iterator.value()["y"], [=](float value)
							{
								data[iterator.key()]["y"] = value;
								behaviour->deserialize(data);
							});
						auto* z = EditorFields::floatField(field, iterator.value()["z"], [=](float value)
							{
								data[iterator.key()]["z"] = value;
								behaviour->deserialize(data);
							});
						auto* w = EditorFields::floatField(field, iterator.value()["w"], [=](float value)
							{
								data[iterator.key()]["w"] = value;
								behaviour->deserialize(data);
							});

						layout->addWidget(x);
						layout->addWidget(y);
						layout->addWidget(z);
						layout->addWidget(w);
					}
					else if (type == TRISTEON_TYPENAME(Tristeon::Vector2Int))
					{
						field = new QWidget(formWidget);
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						auto* x = EditorFields::intField(field, iterator.value()["x"], [=](int value)
							{
								data[iterator.key()]["x"] = value;
								behaviour->deserialize(data);
							});
						auto* y = EditorFields::intField(field, iterator.value()["y"], [=](int value)
							{
								data[iterator.key()]["y"] = value;
								behaviour->deserialize(data);
							});

						layout->addWidget(x);
						layout->addWidget(y);
					}
					else
					{
						field = new QLabel(QString::fromStdString(std::string("Object of type: ") + type + " has no editor (yet)!"));
					}
				}
				else
				{
					field = new QLabel("Objects not supported yet", formWidget);
				}
				//TODO: Support nested objects in property editor
				break;
			}
			case detail::value_t::array:
			{
				//TODO: Support arrays in property editor
				field = new QLabel("Arrays not supported yet", formWidget);
				break;
			}
			case detail::value_t::number_float:
			{
				field = EditorFields::floatField(formWidget, iterator.value(), [=](float value)
					{
						data[iterator.key()] = value;
						behaviour->deserialize(data);
					});
				break;
			}
			case detail::value_t::number_integer:
			{
				field = EditorFields::intField(formWidget, iterator.value(), [=](int value)
					{
						data[iterator.key()] = value;
						behaviour->deserialize(data);
					});
				break;
			}
			case detail::value_t::number_unsigned:
			{
				field = EditorFields::intField(formWidget, iterator.value(), 0, std::numeric_limits<int>::max(), [=](int value)
					{
						data[iterator.key()] = (unsigned int)value;
						behaviour->deserialize(data);
					});
				break;
			}
			default:
				break;
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

	void BehaviourEditor::removeButtonPressed()
	{
		behaviour->destroy();
		parent()->deleteLater();
	}
}
#endif