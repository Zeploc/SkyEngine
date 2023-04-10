// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Engine/Render/Mesh.h"

class TessMesh : public Mesh
{
public:
	TessMesh(float fWidth, float fHeight, glm::vec4 _Colour);

	~TessMesh();

	void Render(FTransform Newtransform) override;

	void Rebind() override; // Will replace if texture exists

	void BindTess();
};
