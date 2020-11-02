#ifdef TRISTEON_EDITOR
#include "Math/Vector.h"
#include "BehaviourEditor.h"
#include "Editor/EditorFields.h"

#include <QtWidgets>
#include <Editor/Editor.h>

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::Behaviour, BehaviourEditor);

	void BehaviourEditor::initialize()
	{
		data = behaviour->serialize();

		QFrame* frame = new QFrame(this);
		frame->setFrameShape(QFrame::Box);
		frame->setFrameShadow(QFrame::Sunken);
		
		frame->setLayout(new QVBoxLayout());
		frame->layout()->setContentsMargins(0, 0, 0, 0);
		frame->setStyleSheet("background-color: rgb(22, 160, 133);");
		layout->addWidget(frame);

		auto* titleBar = new QWidget(frame);
		frame->layout()->addWidget(titleBar);

		auto* titleLayout = new QHBoxLayout(titleBar);
		titleLayout->setContentsMargins(0, 0, 0, 0);
		titleBar->setLayout(titleLayout);

		auto* typeName = new QLabel(QString::fromStdString(data["typeID"]), titleBar);
		typeName->setAlignment(Qt::AlignLeft);
		titleLayout->addWidget(typeName);

		auto* closeButton = new QToolButton(titleBar);
		closeButton->setIcon(QIcon(QPixmap(QString("Internal/Icons/cross.jpg"))));
		closeButton->setMaximumSize(15, 15);
		titleLayout->addWidget(closeButton);
		connect(closeButton, &QToolButton::clicked, this, &BehaviourEditor::removeButtonPressed);

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
		form->setContentsMargins(0, 0, 0, 0);
		formWidget->setLayout(form);

		for (auto it = data.begin(); it != data.end(); ++it)
		{
			std::string key = it.key();

			if (key == "typeID") continue;
			
			switch (it.value().type())
			{
				case detail::value_t::boolean:
				{
					EditorFields::boolField(form, key, data[key], [=](int state)
						{
							data[key] = (bool)((Qt::CheckState)state == Qt::Checked || (Qt::CheckState)state == Qt::PartiallyChecked); behaviour->deserialize(data);
					});
					break;
				}
				case detail::value_t::string:
				{
					EditorFields::stringField(form, key, data[key], [=](std::string value) { data[key] = value; behaviour->deserialize(data); });
					break;
				}
				case detail::value_t::object:
				{
					Tristeon::String const type = it.value().contains("typeID") ? it.value()["typeID"] : "";

					bool const isVector = type == Tristeon::Type<Tristeon::Vector>::fullName();
					bool const isVectorI = type == Tristeon::Type<Tristeon::VectorI>::fullName();

					QWidget* field;
					if (isVector || isVectorI)
					{
						field = new QWidget();
						auto* layout = new QHBoxLayout(field);
						layout->setContentsMargins(0, 0, 0, 0);
						field->setLayout(layout);

						if (isVector)
						{
							auto* x = EditorFields::floatField(field, it.value()["x"], [=](float value) { data[key]["x"] = value; behaviour->deserialize(data); });
							auto* y = EditorFields::floatField(field, it.value()["y"], [=](float value) { data[key]["y"] = value; behaviour->deserialize(data); });
							layout->addWidget(x);
							layout->addWidget(y);
						}

						if (isVectorI)
						{
							auto* x = EditorFields::intField(field, it.value()["x"], [=](int value) { data[key]["x"] = value; behaviour->deserialize(data); });
							auto* y = EditorFields::intField(field, it.value()["y"], [=](int value) { data[key]["y"] = value; behaviour->deserialize(data); });
							layout->addWidget(x);
							layout->addWidget(y);
						}
					}
					else
					{
						//TODO: Support nested objects in behaviour editor
						field = new QLabel("Nested objects aren't supported yet");
					}

					if (field != nullptr)
						form->addRow(new QLabel(QString::fromStdString(key)), field);
					break;
				}
				case detail::value_t::array:
				{
					//TODO: Support arrays in behaviour editor
					EditorFields::labelField(form, key, "Arrays not supported yet");
					break;
				}
				case detail::value_t::number_float:
				{
					EditorFields::floatField(form, key, it.value(), [=](float value){ data[key] = value; behaviour->deserialize(data); });
					break;
				}
				case detail::value_t::number_integer:
				{
					EditorFields::intField(form, key, it.value(), [=](int value) { data[key] = value; behaviour->deserialize(data); });
					break;
				}
				case detail::value_t::number_unsigned:
				{
					EditorFields::intField(form, key, it.value(), 0, std::numeric_limits<int>::max(), [=](int value) { data[key] = (unsigned int)value; behaviour->deserialize(data); });
					break;
				}
				default:
					break;
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