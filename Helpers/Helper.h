
#pragma once

#include <string>
#include "glm/glm.hpp"
#include <iostream>

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

	inline void glm_vec4_to_float4(const glm::vec4& vec, float* arr)
	{
		if(!arr)
			return;

		// as we know size is 4, we don't need to pass it on as argument
		arr[0] = vec.r;
		arr[1] = vec.g;
		arr[2] = vec.b;
		arr[3] = vec.a;
	}
}