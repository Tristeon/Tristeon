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
			registerExtension(fileExtensions...);
		}

		template<typename Extension>
		void registerExtension(Extension extension)
		{
			getMap()->emplace(extension, &CreateInstance<T, MetaFile>);
		}
		
		template<typename Ext, typename ...Args>
		void registerExtension(Ext first, Args... remainder)
		{
			registerExtension(first);
			registerExtension(remainder...);
		}
	};

#define FILE_EXTENSION_TYPE(type, ...) static const FileExtensionRegister<type> extensionRegister_##type(__VA_ARGS__)
}
