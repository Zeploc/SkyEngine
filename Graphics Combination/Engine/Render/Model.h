// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

// Library Includes //
#include <memory>

// Engine Includes //
#include "ModelObject.h"

class Model : public Mesh
{
public:
	Model(glm::vec4 Colour, const char* ModelSource);

	~Model();

	void BindModel();

	void Rebind() override;

	void SetLit(bool _bIsLit);

	void Render(Utils::Transform Newtransform) override;

	void Update() override;

	std::shared_ptr<ModelObject> pModelObject;
};
