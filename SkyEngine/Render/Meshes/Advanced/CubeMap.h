// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Local Includes
#include "Render/Meshes/Mesh.h"

class CubeMap : public Mesh
{
public:
	CubeMap(float fWidth, float fHeight, float fDepth, char* TextureSources[6]);

	~CubeMap();

	void BindCubeMap();

	void Rebind() override;

	// TODO: Update to graphics instance
	void Render(FTransform Newtransform);

	void Update() override;

	char* TextureSources[6];
};
