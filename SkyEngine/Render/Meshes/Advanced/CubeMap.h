// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Local Includes
#include "Render/Meshes/MeshComponent.h"

class CubeMap : public CMeshComponent
{
public:
	CubeMap(const TPointer<Entity>& InOwner, float fWidth, float fHeight, float fDepth, char* TextureSources[6]);

	~CubeMap();

	void BindCubeMap();

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void Update() override;

	char* TextureSources[6];
};
