// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API Pyramid : public Mesh
{
public:
	Pyramid(float fWidth, float fHeight, float fDepth, glm::vec4 Colour);

	Pyramid(float fWidth, float fHeight, float fDepth, glm::vec4 Colour, const char* TextureSource, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));

	~Pyramid();

	void BindPyramid();

	void Rebind() override;


	void Update() override;
};
