#ifdef TRISTEON_EDITOR
#include "Scenes/Layers/ActorLayer.h"
#include "Serialization/Register.h"
#include <QApplication>
#include <qclipboard.h>
#include <qshortcut.h>

#include <Editor/Editor.h>
#include <Editor/Dynamic/Scene/SceneEditor.h>

namespace TristeonEditor
{
	SceneEditor::SceneEditor()
	{
		QShortcut* copy = new QShortcut(QKeySequence(tr("Ctrl+C", "Copy")), this);
		connect(copy, &QShortcut::activated, this, [&]()
			{
				if (Editor::instance()->selectedActor() != nullptr)
					QApplication::clipboard()->setText(QString::fromStdString(Editor::instance()->selectedActor()->serialize().dump(4)));
			});

		QShortcut* paste = new QShortcut(QKeySequence(tr("Ctrl+V", "Paste")), this);
		connect(paste, &QShortcut::activated, this, [&]()
			{
				auto* selectedLayer = Editor::instance()->selectedLayer();
				auto* actorLayer = dynamic_cast<Tristeon::ActorLayer*>(selectedLayer);

				if (actorLayer)
				{
					const auto text = QApplication::clipboard()->text().toStdString();
					const auto j = json::parse(text, nullptr, false);
					const auto type = j.value("typeID", std::string());
					if (Tristeon::Register<Tristeon::Actor>::registered(type))
					{
						auto* actor = actorLayer->createActor(type);
						actor->deserialize(j);
					}
				}
			});
	}

	void SceneEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		currentLayer = (Tristeon::Layer*)current;
	}
}
#endif