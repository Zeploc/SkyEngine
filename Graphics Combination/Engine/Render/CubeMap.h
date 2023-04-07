// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Local Includes
#include "Mesh.h"

class CubeMap : public Mesh
{
public:
	CubeMap(float fWidth, float fHeight, float fDepth, char* TextureSources[6]);

	~CubeMap();

	void BindCubeMap();

	void Rebind() override;

	void Render(Utils::Transform Newtransform) override;

	void Update() override;

	char* TextureSources[6];
};
