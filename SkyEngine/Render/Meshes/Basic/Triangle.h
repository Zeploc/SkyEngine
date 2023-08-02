// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class CTriangle : public CMeshComponent
{
public:
	CTriangle(const TPointer<Entity>& InOwner, glm::vec3 _Point1, glm::vec3 _Point2, glm::vec3 _Point3, TPointer<CMaterial> InMaterial = nullptr);
	CTriangle(const TPointer<Entity>& InOwner, glm::vec3 CenterPoint, float Width, TPointer<CMaterial> InMaterial = nullptr);

	~CTriangle();
	
protected:
	MeshData GetMeshData() override;
	
	glm::vec3 Point1;
	glm::vec3 Point2;
	glm::vec3 Point3;
};
