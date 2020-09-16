#pragma once
#include "Utils/TypeDefinitions.h"
#include <filesystem>

namespace TristeonEditor
{
	class Importer
	{
	public:
		Importer() = default;

		void import(std::filesystem::path assetPath);

		void scanProject();
	};
}