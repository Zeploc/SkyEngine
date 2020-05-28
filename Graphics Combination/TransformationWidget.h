
#pragma once
#include "Engine\Entity.h"

class TransformationWidget : public Entity
{
public:
	TransformationWidget(Utils::Transform _Transform, class Scene* OwningScene);
	~TransformationWidget();

	virtual void Update() override;
	virtual void DrawEntity() override;

	std::shared_ptr<Entity> XMoveTransform;
	std::shared_ptr<Entity> YMoveTransform;
	std::shared_ptr<Entity> ZMoveTransform;
	
	bool XHit = false;
	bool YHit = false;
	bool ZHit = false;

	glm::vec2 PreviousMouse;
	glm::vec3 HitPosition;
	std::shared_ptr<Entity> SelectedEntity;
};

