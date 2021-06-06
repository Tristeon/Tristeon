#pragma once
#include "MetaFile.h"
#include <AssetManagement/FileExtensionRegister.h>

namespace Tristeon
{
	/**
	 * @brief SerializableMetaFile is for objects that are stored as json files.
	 * These objects are recognized by their serialized typeID and created as such through load().
	*/
	class SerializableMetaFile : public MetaFile
	{
	public:
		/**
		 * @brief Reads the corresponding json file, it then uses TypeRegister::createInstance to create the object and calls deserialize() on the said object.
		*/
		Unique<TObject> load(const String& path) override;
		json serialize() override;
	};

	REGISTER_TYPE(SerializableMetaFile);
	FILE_EXTENSION_TYPE(SerializableMetaFile, ".ser", ".clip");
}
