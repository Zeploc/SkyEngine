// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/Mesh.h"

// Library Includes //
#include <memory>

// Engine Includes //

class ModelObject;

class Model : public CMeshComponent
{
public:
	Model(const TPointer<Entity>& InOwner, glm::vec4 Colour, const char* ModelSource);

	~Model();

	void BindModel();

	void Rebind() override;

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void Update() override;

	TPointer<ModelObject> pModelObject;

	std::string ModelPath;
};
