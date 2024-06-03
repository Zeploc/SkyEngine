// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

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
	
	uint32_t GetTextureRenderID() const
	{
		return TextureID;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const TPointer<CTexture>& InTexture)
	{
		os << InTexture->Path;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, TPointer<CTexture>& OutTexture)
	{
		is >> OutTexture->Path;
		return is;
	}

protected:
	
};