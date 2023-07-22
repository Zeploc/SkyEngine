// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library //
#include <glew/glew.h>

//#include <freeglut.h>

// Local Libraries //
#include "Math/FTransform.h"

struct ENGINE_API LightInfo
{
	glm::vec3 v3AmbientColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 v3LightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	float fAmbientStrength = 0.1f;
	float fLightSpecStrength = 1.0f;
	float fShininess = 32.0f;
};

class ENGINE_API Lighting
{
public:
	Lighting();

	~Lighting();

	// static Vector3 LightPosition;
	// static Vector3 SunDirection;
	static glm::vec4 m_v4FogColour;
	static float StartFogDistance;
	static float EndFogDistance;

	static void PassLightingToShader(GLuint program, LightInfo _LightInfo, FTransform ModelTransform);
};
