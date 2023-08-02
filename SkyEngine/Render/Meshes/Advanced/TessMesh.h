// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class TessMesh : public CMeshComponent
{
public:
	TessMesh(float fWidth, float fHeight, glm::vec4 _Colour);

	~TessMesh();

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void Rebind() override; // Will replace if texture exists

	void BindTess();
};
