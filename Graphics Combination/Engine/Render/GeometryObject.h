// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class GeometryObject : public Mesh
{
public:
	GeometryObject(glm::vec4 Colour);

	~GeometryObject();

	void BindGeometryObject();

	void Rebind() override;

	void Render(Utils::Transform Newtransform) override;
};
