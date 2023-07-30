// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

// Library Includes //
#include <memory>

// Engine Includes //

class ModelObject;

class Model : public Mesh
{
public:
	Model(glm::vec4 Colour, const char* ModelSource);

	~Model();

	void BindModel();

	void Rebind() override;

	void SetLit(bool _bIsLit);

	// TODO: Update to graphics instance
	void Render(FTransform Newtransform);

	void Update() override;

	Pointer<ModelObject> pModelObject;
};
