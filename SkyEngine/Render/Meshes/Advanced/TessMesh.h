// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

class TessMesh : public CMeshComponent
{
public:
	TessMesh(const THardPointer<Entity>& InOwner,float fWidth, float fHeight, glm::vec4 _Colour);

	~TessMesh();

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void BindTess();
};
