// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class ENGINE_API PathUtils
{
public:
	static std::string GetFileName(const std::string& Path, bool bWithExtension = true);
	/* Get directory path without file name */
	static std::string GetDirectory(const std::string& Path);
	/* Take two path segments joined by a / separator if one is not present */
	static std::string CombinePath(const std::string& Part1, const std::string& Part2);
	/* Make the slashes consistent with the standard */
	static std::string GetNormalizePath(const std::string& Path);
	/* Make the slashes consistent with the standard */
	static void NormalizePath(std::string& Path);

	static bool IsRelativeToPath(const std::string& Path, const std::string& RelativeTo);
	static std::string GetRelativePath(const std::string& Path, const std::string& RelativeTo);

	/* Applies the extension (adding if one doesn't exit), give extension with the dot */
	static void SetExtension(std::string& Path, const std::string& Extension);
	/* Checks given file path has an extension (*.ext) */
	static bool HasExtension(const std::string& Path);
	/* Returns the extension of the file (*.ext) or empty if it doesn't have one */
	static std::string GetExtension(const std::string& Path);
};
