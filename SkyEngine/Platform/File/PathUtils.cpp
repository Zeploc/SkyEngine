#include "SEPCH.h"
#include "PathUtils.h"

#include "Core/StringUtils.h"

std::string PathUtils::GetFileName(const std::string& Path, bool bWithExtension)
{
	const std::string NormalizedPath = GetNormalizePath(Path);
	const uint64_t DirectoryEndIndex = NormalizedPath.find_last_of("\\");
	std::string FileName = NormalizedPath.substr(DirectoryEndIndex + 1);
	if (!bWithExtension && HasExtension(Path))
	{
		const uint64_t ExtensionEndIndex = FileName.find_last_of(".");
		std::string BaseName = FileName.substr(0, ExtensionEndIndex);
		return BaseName;
	}
	return FileName;
}

std::string PathUtils::GetDirectory(const std::string& Path)
{
	const uint64_t DirectoryEndIndex = Path.find_last_of("\\");
	return Path.substr(0, DirectoryEndIndex + 1);
}

bool PathUtils::HasExtension(const std::string& Path)
{
	const std::string FileName = GetFileName(Path);
	const uint64_t ExtensionEndIndex = FileName.find_last_of(".");
	return ExtensionEndIndex < FileName.length() - 1;
}

std::string PathUtils::CombinePath(const std::string& Part1, const std::string& Part2)
{
	if (Part1.ends_with("\\"))
	{
		return Part1 + Part2;
	}
	return Part1 + std::string("\\") + Part2;
}

std::string PathUtils::GetNormalizePath(const std::string& Path)
{
	std::string Normalized = Path;
	NormalizePath(Normalized);
	return Normalized;
}

void PathUtils::NormalizePath(std::string& Path)
{
	StringUtils::Replace(Path, "/", "\\");
}

void PathUtils::SetExtension(std::string& Path, const std::string& Extension)
{
	if (HasExtension(Path))
	{
		// TODO: Improve
		Path = GetDirectory(Path) + GetFileName(Path, false);
	}
	Path += "." + Extension;
}
