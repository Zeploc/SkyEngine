// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class Cube : public Mesh
{
public:
	Cube(float fWidth, float fHeight, float fDepth, glm::vec4 Colour);

	Cube(float fWidth, float fHeight, float fDepth, glm::vec4 Colour, const char* TextureSource, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));

	~Cube();

	void BindCube();

	void Rebind() override;

	void SetLit(bool _bIsLit);

	void Render(Utils::Transform Newtransform) override;

	void Update() override;
};
