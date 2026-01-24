// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Library //
#include "Entity/Entity.h"

// OpenGL Library //

class CCollisionBounds
{
public:
	CCollisionBounds(float fHeight, float fWidth, float fDepth, TSharedPointer<Entity> _EntityRef);

	~CCollisionBounds();

	void Reset();

	void SetOffset(glm::vec3 _NewOffset);

	bool isColliding(TSharedPointer<Entity> Entity2);

	bool CheckCollision(TSharedPointer<Entity> Entity2, glm::vec3 Movement);

	glm::vec3 GetDistance(TSharedPointer<Entity> Entity2);

	glm::vec3 GetDimensions() { return glm::vec3(fWidth, fHeight, fDepth); };
	glm::vec3 GetOffset() { return v3Offset; };
	TSharedPointer<Entity> EntityRef;
private:
	float fHeight;
	float fWidth;
	float fDepth;
	glm::vec3 v3Offset = glm::vec3();
};
