#pragma once
#include "MetaFiles/MetaFile.h"
#include "Serialization/Register.h"

namespace Tristeon
{
	template<typename T>
	class FileExtensionRegister : Register<MetaFile>
	{
	public:
		template<typename ...Extensions>
		FileExtensionRegister(Extensions... fileExtensions)
		{
			getMap()->emplace(fileExtensions..., &CreateInstance<T, MetaFile>);
		}
	};

#define FILE_EXTENSION_TYPE(type, ...) static const FileExtensionRegister<type> extensionRegister_##type(__VA_ARGS__)
}
