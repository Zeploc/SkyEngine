// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class ENGINE_API PathUtils
{
public:
	static std::string GetFileName(const std::string& Path, bool bWithExtension = true);
	static std::string GetDirectory(const std::string& Path);	
	static bool HasExtension(const std::string& Path);

	/* Applies the extension (adding if one doesn't exit), give extension with the dot */
	static void SetExtension(std::string& Path, const std::string& Extension);
};
