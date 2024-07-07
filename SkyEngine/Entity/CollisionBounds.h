// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //

// Engine Library //
#include "Entity.h"

// OpenGL Library //

class CCollisionBounds
{
public:
	CCollisionBounds(float fHeight, float fWidth, float fDepth, THardPointer<Entity> _EntityRef);

	~CCollisionBounds();

	void Reset();

	void SetOffset(glm::vec3 _NewOffset);

	bool isColliding(THardPointer<Entity> Entity2);

	bool CheckCollision(THardPointer<Entity> Entity2, glm::vec3 Movement);

	glm::vec3 GetDistance(THardPointer<Entity> Entity2);

	glm::vec3 GetDimensions() { return glm::vec3(fWidth, fHeight, fDepth); };
	glm::vec3 GetOffset() { return v3Offset; };
	THardPointer<Entity> EntityRef;
private:
	float fHeight;
	float fWidth;
	float fDepth;
	glm::vec3 v3Offset = glm::vec3();
};
