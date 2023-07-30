// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Mesh.h"

class TessMesh : public Mesh
{
public:
	TessMesh(float fWidth, float fHeight, glm::vec4 _Colour);

	~TessMesh();

	// TODO: Update to graphics instance
	void Render(FTransform Newtransform);

	void Rebind() override; // Will replace if texture exists

	void BindTess();
};
