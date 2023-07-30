// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

struct TextureData
{
	std::string Path;
	unsigned int TextureID = 0;
	int Width = 0;
	int Height = 0;

	bool IsValid() const
	{
		return !Path.empty();
	}
};