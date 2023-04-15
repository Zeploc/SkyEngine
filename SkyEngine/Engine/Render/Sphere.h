// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class Sphere : public Mesh
{
public:
	Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 Colour);

	Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 Colour, const char* TextureSource, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));

	~Sphere();

	void BindSphere();

	void Rebind() override;

	void SetLit(bool _bIsLit);

	void Render(FTransform Newtransform) override;

	void Update() override;
};
