// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// TODO: Use platform interface for base file operations
class ENGINE_API CFileManager
{
public:
	static bool OpenFile(std::string& OpenedFile);
	static bool ReadFile(const std::string& FilePath, std::string& FileContents);
	static bool SaveFile(const std::string& FilePath, std::string FileContents);
	static bool SaveAsFile(std::string& ChosenFilePath, std::string FileExtension, std::string FileName = std::string(), std::string FileDirectory = std::string());
};
