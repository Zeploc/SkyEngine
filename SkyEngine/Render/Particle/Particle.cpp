// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Particle.h"

// Engine Includes //
#include "System/TimeManager.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Particle::Particle(float _fSpeed, glm::vec3 _v3Direction, float _fFalloffDistance, float _fFallOffTime, float _fTimeLength, glm::vec3 _InitialPosition, glm::vec4 _Colour)
{
	fSpeed = _fSpeed;
	v3Direction = _v3Direction;
	fFalloffDistance = _fFalloffDistance;
	fFallOffTime = _fFallOffTime;
	fTimeLength = _fTimeLength;
	fElapsedTime = 0;

	position = _InitialPosition;
	InitialPosition = position;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Particle::~Particle()
{
}

void Particle::Update()
{
	fElapsedTime += TimeManager::GetDeltaTime();
	float Distance = length(position - InitialPosition);
	if (fElapsedTime > fFallOffTime || Distance > fFalloffDistance)
	{
		position = InitialPosition;
		fElapsedTime = 0.0f;
	}
	position += v3Direction * fSpeed * TimeManager::GetDeltaTime();
}
