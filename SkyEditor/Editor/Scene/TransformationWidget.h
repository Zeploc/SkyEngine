// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Entity/Entity.h>
#include <System/Utils.h>

#include "Math/Vector4.h"

class TransformationWidget : public Entity
{
public:
	TransformationWidget(STransform _Transform, class Scene* OwningScene);

	~TransformationWidget();

	void Update() override;

	std::vector<TPointer<Entity>> GetAdditionalEntitiesToRender() override;

	TPointer<Entity> XMoveTransform;
	TPointer<Entity> YMoveTransform;
	TPointer<Entity> ZMoveTransform;

	bool XHovered = false;
	bool YHovered = false;
	bool ZHovered = false;
	bool XHeld = false;
	bool YHeld = false;
	bool ZHeld = false;

	float GrabOffset = 0.0f;

	SVector HitPosition;
	TPointer<Entity> SelectedEntity;
	
	SVector4 XColour = {0.6f, 0.1f, 0.1f, 1.0f};
	SVector4 YColour = {0.1f, 0.6f, 0.1f, 1.0f};
	SVector4 ZColour = {0.1f, 0.1f, 0.6f, 1.0f};
};
