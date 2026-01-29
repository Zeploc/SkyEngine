// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"
#include "Core/Asset/AssetObject.h"
#include "Math/Transform.h"

class CMaterialInterface;

class ISceneVisual
{
public:
	
	virtual bool ShouldRender() const = 0;

	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool bNewVisible) = 0;
	virtual bool ShouldRenderFaces() const = 0;

	// TODO: Should this be in the shader
	virtual TAssetObjectPointer<CMaterialInterface> GetMaterial() const = 0;

	virtual STransform GetRenderTransform() const = 0;
	
	virtual uint32_t GetVao() const = 0;
	virtual int GetIndicesCount() const = 0;
};
