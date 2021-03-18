#ifdef TRISTEON_EDITOR
#include "GraphicEditor.h"
#include "Editor/EditorFields.h"
#include <qformlayout.h>
#include <magic_enum.hpp>

namespace TristeonEditor
{
	void GraphicEditor::targetChanged(Tristeon::TObject* pCurrent, Tristeon::TObject* pOld)
	{
		ActorEditor::targetChanged(pCurrent, pOld);
		_graphic = (Tristeon::Graphic*)pCurrent;
	}

	void GraphicEditor::displayProperties()
	{
		ActorEditor::displayProperties();
		
		auto* formWidget = new QWidget(scrollArea);
		auto* form = new QFormLayout(formWidget);
		formWidget->setLayout(form);
		scrollLayout->addWidget(formWidget);

		constexpr const auto& names = magic_enum::enum_names<Tristeon::RenderMask>();
		Tristeon::List<Tristeon::String> options;
		for (auto name : names)
		{
			if ((Tristeon::String)name != "None")
				options.add((Tristeon::String)name);
		}

		auto current = magic_enum::enum_index<Tristeon::RenderMask>(_graphic->renderMask);
		if (_graphic->renderMask == Tristeon::RenderMask::All)
			current = 9;
		EditorFields::dropdown(form, "Render Mask", current.has_value() ? (int)current.value() : (int)Tristeon::RenderMask::All, options, [=](int idx)
			{
				_graphic->renderMask = magic_enum::enum_value<Tristeon::RenderMask>(idx);
				if (idx == 9)
					_graphic->renderMask = Tristeon::RenderMask::All;
			});
	}
}
#endif