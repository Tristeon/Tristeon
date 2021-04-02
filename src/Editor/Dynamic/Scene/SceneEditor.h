#pragma once
#include <qwidget.h>

#include <Scenes/Layers/Layer.h>
#include <Utils/ClassDefaults.h>

namespace TristeonEditor
{
	class SceneEditor : public QWidget
	{
	public:
		SceneEditor(Tristeon::Layer* pLayer);
		virtual ~SceneEditor() = default;

		virtual void updateView() = 0;

		DELETE_COPY(SceneEditor);
		DEFAULT_MOVE(SceneEditor);
		
	protected:
		Tristeon::Layer* _layer = nullptr;
	};
}