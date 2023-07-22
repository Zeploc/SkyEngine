// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <memory>

// Engine Library //
#include "Entity.h"

// OpenGL Library //

class CollisionBounds
{
public:
	CollisionBounds(float fHeight, float fWidth, float fDepth, std::shared_ptr<Entity> _EntityRef);

	~CollisionBounds();

	void Reset();

	void SetOffset(glm::vec3 _NewOffset);

	bool isColliding(std::shared_ptr<Entity> Entity2);

	bool CheckCollision(std::shared_ptr<Entity> Entity2, glm::vec3 Movement);

	glm::vec3 GetDistance(std::shared_ptr<Entity> Entity2);

	glm::vec3 GetDimensions() { return glm::vec3(fWidth, fHeight, fDepth); };
	glm::vec3 GetOffset() { return v3Offset; };
	std::shared_ptr<Entity> EntityRef;
private:
	float fHeight;
	float fWidth;
	float fDepth;
	glm::vec3 v3Offset = glm::vec3();
};
