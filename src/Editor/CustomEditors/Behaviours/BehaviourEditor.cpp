#ifdef TRISTEON_EDITOR
#include "BehaviourEditor.h"
#include <QtWidgets>

namespace TristeonEditor
{
	CUSTOM_EDITOR_CPP(Tristeon::Behaviour, BehaviourEditor)

	void BehaviourEditor::initialize()
	{
		data = behaviour->serialize();

		auto* titleBar = new QWidget(this);
		layout->addWidget(titleBar);

		QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
		titleLayout->setContentsMargins(0, 0, 0, 0);
		titleBar->setLayout(titleLayout);

		QLabel* typeName = new QLabel(QString::fromStdString(data["typeID"]), titleBar);
		typeName->setAlignment(Qt::AlignLeft);
		titleLayout->addWidget(typeName);

		QToolButton* closeButton = new QToolButton(titleBar);
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
				//TODO: Support nested objects in property editor
				field = new QLabel("Objects not supported yet", formWidget);
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
				auto* floatField = new QDoubleSpinBox(formWidget);
				floatField->setFocusPolicy(Qt::StrongFocus);
				floatField->setSingleStep(0);
				floatField->setButtonSymbols(QDoubleSpinBox::NoButtons);
				floatField->setMinimum(std::numeric_limits<float>::min());
				floatField->setMaximum(std::numeric_limits<float>::max());
				floatField->setValue(iterator.value().get<float>());
				floatField->show();

				field = floatField;
				connect(floatField, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double value)
					{
						data[iterator.key()] = (float)value;
						behaviour->deserialize(data);
					});
				break;
			}
			case detail::value_t::number_integer:
			{
				auto* intField = new QSpinBox(formWidget);
				intField->setFocusPolicy(Qt::StrongFocus);
				intField->setMinimum(std::numeric_limits<int>::min());
				intField->setMaximum(std::numeric_limits<int>::max());
				intField->setSingleStep(0);
				intField->setButtonSymbols(QDoubleSpinBox::NoButtons);
				intField->setValue(iterator.value().get<int>());
				intField->show();

				field = intField;

				connect(intField, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value)
					{
						data[iterator.key()] = value;
						behaviour->deserialize(data);
					});

				break;
			}
			case detail::value_t::number_unsigned:
			{
				auto* unsignedField = new QSpinBox(formWidget);
				unsignedField->setFocusPolicy(Qt::StrongFocus);
				unsignedField->setMinimum(0);
				unsignedField->setMaximum(std::numeric_limits<int>::max());
				unsignedField->setSingleStep(0);
				unsignedField->setButtonSymbols(QDoubleSpinBox::NoButtons);
				unsignedField->setValue(iterator.value().get<int>());
				unsignedField->show();

				field = unsignedField;

				connect(unsignedField, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](int value)
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