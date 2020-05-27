#ifdef TRISTEON_EDITOR
#include "AnimationSpriteEditor.h"

namespace TristeonEditor
{
	OBJECT_EDITOR_CPP(Tristeon::AnimationSprite, AnimationSpriteEditor);

	void AnimationSpriteEditor::targetChanged(Tristeon::TObject* current, Tristeon::TObject* old)
	{
		SpriteEditor::targetChanged(current, old);
		animSprite = (Tristeon::AnimationSprite*)current;
	}

	bool AnimationSpriteEditor::shouldDisplay(Tristeon::String const& propertyName)
	{
		if (propertyName == "clipPath")
			return false;
		return SpriteEditor::shouldDisplay(propertyName);
	}

	void AnimationSpriteEditor::textureButton()
	{
		QPushButton* button = new QPushButton(this);
		button->setText("Load Animation Clip");
		connect(button, &QPushButton::clicked, this, &AnimationSpriteEditor::loadAnimationClip);
		scrollLayout->addWidget(button);
	}

	void AnimationSpriteEditor::loadAnimationClip()
	{
		QDir const baseDir(QDir::currentPath());

		QString const path = QFileDialog::getOpenFileName(this, tr("Find Animation Clip"), QDir::currentPath() + "/Project", "*.clip");
		QString const localPath = baseDir.relativeFilePath(path);
		QString const fileName = QFileInfo(path).baseName();
		if (path.isEmpty() || localPath.isEmpty())
			return;

		animSprite->setAnimationClip(localPath.toStdString());
	}
}
#endif