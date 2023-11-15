// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

class StringUtils
{
public:
	static bool ReplaceFirst(std::string& InString, const std::string& From, const std::string& To);
	static void Replace(std::string& InString, const std::string& From, const std::string& To);
	static std::wstring ToWString(const std::string& InString);
};
