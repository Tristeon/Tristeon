#pragma once
#include "MetaFile.h"
#include "Serialization/TypeRegister.h"

namespace Tristeon
{
	class TextureMetaFile : public MetaFile
	{
	public:
		json serialize() override;

		void deserialize(json j) override;

		Unique<TObject> load() override;
	};

	REGISTER_TYPE(TextureMetaFile);
}
