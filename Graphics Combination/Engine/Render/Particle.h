//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    ParticleSystem.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

#pragma once

// Local Includes //
#include "Mesh.h"

class Particle
{
public:
	Particle(float fSpeed, glm::vec3 v2Direction, float fFalloffDistance, float fFallOffTime, float fTimeLength, glm::vec3 _InitialPosition = { 0, 0, 0 }, glm::vec4 _Colour = { 1.0f, 1.0f, 1.0f, 1.0f });
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

