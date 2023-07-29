// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class ENGINE_API Sphere : public Mesh
{
public:
	Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 Colour);

	Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 Colour, const char* TextureSource, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));

	~Sphere();

	void BindSphere();

	void Rebind() override;

	void SetLit(bool _bIsLit) override;

	void Render(FTransform Newtransform) override;

	void Update() override;
	
	bool CheckHit(Vector3 RayStart, Vector3 RayDirection, Vector3& HitPos, Pointer<Entity> EntityCheck) override;
};
