// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>

class CTexture
{
public:
	std::string Path;
	int Width = 0;
	int Height = 0;
	uint32_t TextureID = 0;
	bool bBound = false;

	// TODO:
	bool bAntiAliasing = true;

	CTexture() {}
	CTexture(const std::string InPath)
	{
		Path = InPath;
	}

	bool IsValid() const
	{
		return !Path.empty() && IsBound();
	}
	bool IsBound() const
	{
		return bBound;
	}
	void Bind(const uint32_t InTextureID)
	{
		TextureID = InTextureID;
		bBound = true;
	}

protected:
	
};