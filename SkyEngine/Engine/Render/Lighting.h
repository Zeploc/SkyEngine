// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// OpenGL Library //
#include <glew.h>

//#include <freeglut.h>

// Local Libraries //
#include "Engine/Math/FTransform.h"
#include "Engine/System/Utils.h"

struct LightInfo
{
	glm::vec3 v3AmbientColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 v3LightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	float fAmbientStrength = 0.1f;
	float fLightSpecStrength = 1.0f;
	float fShininess = 32.0f;
};

#pragma once
class Lighting
{
public:
	Lighting();

	~Lighting();

	static glm::vec3 m_v3LightPosition;
	static glm::vec3 m_v3SunDirection;
	static glm::vec4 m_v4FogColour;
	static float StartFogDistance;
	static float EndFogDistance;

	static void PassLightingToShader(GLuint program, LightInfo _LightInfo, FTransform ModelTransform);
};
