// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

class CTriangle : public CMeshComponent
{
public:
	CTriangle(const THardPointer<Entity>& InOwner, glm::vec3 _Point1, glm::vec3 _Point2, glm::vec3 _Point3, THardPointer<CMaterialInterface> InMaterial = nullptr);
	CTriangle(const THardPointer<Entity>& InOwner, glm::vec3 CenterPoint, float Width, THardPointer<CMaterialInterface> InMaterial = nullptr);

	~CTriangle();
	
protected:
	CMeshData GetMeshData() const override;
	
	glm::vec3 Point1;
	glm::vec3 Point2;
	glm::vec3 Point3;
};
