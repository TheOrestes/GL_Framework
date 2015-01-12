
#pragma once

#include <string>

namespace Helper
{
	/// Extract directory path from given filepath
	inline std::string ExtractDirectory(const std::string& path)
	{
		return path.substr(0, path.find_last_of('/') + 1);
	}

	/// Extract raw filename from the given filepath
	inline std::string ExtractRawFileName(const std::string& fileName)
	{
		int lastIndex = fileName.find_last_of(".");
		std::string rawName = fileName.substr(0, lastIndex);

		return rawName;
	}
}