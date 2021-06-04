#include "TextureMetaFile.h"
#include <Rendering/Texture.h>

namespace Tristeon
{
	json TextureMetaFile::serialize()
	{
		json j = MetaFile::serialize();
		j["typeID"] = Type<TextureMetaFile>::fullName();
		return j;
	}

	void TextureMetaFile::deserialize(json j)
	{
		MetaFile::deserialize(j);
	}

	Unique<TObject> TextureMetaFile::load(const String& path)
	{
		return std::make_unique<Texture>(path);
	}
}
