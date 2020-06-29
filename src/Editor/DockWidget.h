#pragma once
#ifdef TRISTEON_EDITOR
namespace Tristeon { class Actor; class Scene; class Layer; }

namespace TristeonEditor
{
	struct FileItem;
	
	class DockWidget
	{
		friend class Editor;
	public:
		DockWidget() = default;
		virtual ~DockWidget() = default;

	protected:
		virtual void initialize() = 0;
		virtual void sceneLoaded(Tristeon::Scene* scene) = 0;
		virtual void selectedLayerChanged(Tristeon::Layer* layer) { /* Empty */ }
		virtual void selectedActorChanged(Tristeon::Actor* actor) { /* Empty */ }
		virtual void selectedFilePathChanged(FileItem* item) { /* Empty */ }
	};
}
#endif