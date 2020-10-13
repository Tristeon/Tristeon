#pragma once
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