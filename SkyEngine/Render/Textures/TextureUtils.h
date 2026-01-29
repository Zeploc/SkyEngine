// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"

class CTexture;

class ENGINE_API CTextureUtils
{
public:
	/* Creates a new texture asset and object based on the given texture path
	 * Returns new the object if successful
	 */
	static TAssetObjectPointer<CTexture> AddNewTexture(const std::string& TexturePath);

	/* Try finding a texture asset object based on the source path*/
	static TAssetObjectPointer<CTexture> FindTextureBySource(const std::string& TextureSourcePath);
};
