// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Local Includes //
#include "Render/Meshes/Mesh.h"

class ENGINE_API Particle
{
public:
	Particle(float fSpeed, glm::vec3 v2Direction, float fFalloffDistance, float fFallOffTime, float fTimeLength, glm::vec3 _InitialPosition = {0, 0, 0}, glm::vec4 _Colour = {1.0f, 1.0f, 1.0f, 1.0f});

	~Particle();

	void Update();

	glm::vec3 position;

private:
	float fSpeed;
	glm::vec3 v3Direction;
	float fFalloffDistance;
	float fFallOffTime;
	float fElapsedTime;
	float fTimeLength;

	glm::vec3 InitialPosition;
};
