// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

class Triangle : public Mesh
{
public:
	Triangle(glm::vec3 _Point1, glm::vec3 _Point2, glm::vec3 _Point3, glm::vec4 Colour);

	Triangle(glm::vec3 CenterPoint, float Width, glm::vec4 Colour);

	~Triangle();

	void BindTriangle();

	void Rebind() override;

	void Update() override;

	glm::vec3 Point1;
	glm::vec3 Point2;
	glm::vec3 Point3;
};
