// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

class ENGINE_API StringUtils
{
public:
	static bool ReplaceFirst(std::string& InString, const std::string& From, const std::string& To);
	static void Replace(std::string& InString, const std::string& From, const std::string& To);
	static std::wstring ToWString(const std::string& InString);
	static std::string NormalizePath(const std::string& MessyPath);
	static void LTrim(std::string& s);
	static void RTrim(std::string& s);
	static void Trim(std::string& s);
	static std::string LTrim_Copy(std::string s);
	static std::string RTrim_Copy(std::string s);
	static std::string Trim_Copy(std::string s);
};
