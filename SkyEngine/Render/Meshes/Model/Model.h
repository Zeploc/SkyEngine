// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Render/Meshes/MeshComponent.h"

// Library Includes //

// Engine Includes //

class ModelObject;

class Model : public CMeshComponent
{
public:
	Model(const TSharedPointer<Entity>& InOwner, glm::vec4 Colour, const char* ModelSource);

	~Model();

	void BindModel();

	// TODO: Update to graphics instance
	void Render(STransform Newtransform);

	void Update() override;

	TSharedPointer<ModelObject> pModelObject;

	std::string ModelPath;
};
