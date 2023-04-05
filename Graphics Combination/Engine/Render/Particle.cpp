//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    ParticleSystem.cpp
// Description    	:    main implementation for ParticleSystem
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//


// This Includes //
#include "Particle.h"

// Engine Includes //
#include "Engine/System/Time.h"

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
	fElapsedTime += Time::dTimeDelta;
	float Distance = glm::length(position - InitialPosition);
	if (fElapsedTime > fFallOffTime || Distance > fFalloffDistance)
	{
		position = InitialPosition;
		fElapsedTime = 0.0f;
	}
	position += v3Direction * fSpeed * (float)Time::dTimeDelta;
}

