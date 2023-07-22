// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Entity/Entity.h>
#include <System/Utils.h>

class TransformationWidget : public Entity
{
public:
	TransformationWidget(FTransform _Transform, class Scene* OwningScene);

	~TransformationWidget();

	void Update() override;

	void DrawEntity() override;

	std::shared_ptr<Entity> XMoveTransform;
	std::shared_ptr<Entity> YMoveTransform;
	std::shared_ptr<Entity> ZMoveTransform;

	bool XHovered = false;
	bool YHovered = false;
	bool ZHovered = false;
	bool XHeld = false;
	bool YHeld = false;
	bool ZHeld = false;

	float GrabOffset = 0.0f;

	Vector3 HitPosition;
	std::shared_ptr<Entity> SelectedEntity;
	
	Vector4 XColour = {0.6f, 0.1f, 0.1f, 1.0f};
	Vector4 YColour = {0.1f, 0.6f, 0.1f, 1.0f};
	Vector4 ZColour = {0.1f, 0.1f, 0.6f, 1.0f};
};
