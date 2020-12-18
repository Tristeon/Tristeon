#pragma once
#include <AssetManagement/MetaFiles/MetaFile.h>
#include "Serialization/TypeRegister.h"
#include <AssetManagement/FileExtensionRegister.h>

namespace Tristeon
{
	class AudioMetaFile : public MetaFile
	{
	public:
		json serialize() override;
		
		void deserialize(json j) override;

		Unique<TObject> load() override;
	};

	REGISTER_TYPE(AudioMetaFile);
	FILE_EXTENSION_TYPE(AudioMetaFile, ".wav");
}