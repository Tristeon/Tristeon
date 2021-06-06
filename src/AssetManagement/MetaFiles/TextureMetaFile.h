#pragma once
#include "MetaFile.h"
#include <Serialization/TypeRegister.h>
#include <AssetManagement/FileExtensionRegister.h>

namespace Tristeon
{
	class TextureMetaFile : public MetaFile
	{
	public:
		json serialize() override;
		Unique<TObject> load(const String& path) override;
	};

	REGISTER_TYPE(TextureMetaFile);
	FILE_EXTENSION_TYPE(TextureMetaFile,".png", ".jpg", ".jpeg", ".bmp", ".tga", ".psd");
}
