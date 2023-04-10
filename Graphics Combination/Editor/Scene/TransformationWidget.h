// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/Entity/Entity.h"
#include "Engine/System/Utils.h"

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

	bool XHit = false;
	bool YHit = false;
	bool ZHit = false;

	Vector3 HitPosition;
	std::shared_ptr<Entity> SelectedEntity;
};
