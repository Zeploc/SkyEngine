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
	Vector3 AmbientColour = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 LightColour = Vector3(1.0f, 1.0f, 1.0f);
	float fAmbientStrength = 0.1f;
	float fLightSpecStrength = 1.0f;
	float fShininess = 32.0f;
};

class ENGINE_API Lighting
{
public:
	Lighting();
	~Lighting();

	static void PassLightingToShader(GLuint program, LightInfo _LightInfo, FTransform ModelTransform);

	// Can't export static member variables directly if class is exported
	static inline Vector3 GetLightPosition();
	static inline Vector3 GetSunDirection();
	static inline glm::vec4 GetFogColour();
	static inline float GetStartFogDistance();
	static inline float GetEndFogDistance();
	static inline void SetLightPosition(Vector3 InLightPosition);
	static inline void SetSunDirection(Vector3 InSunDirection);
	static inline void SetFogColour(glm::vec4 InFogColour);

private:
	static Vector3 LightPosition;
	static Vector3 SunDirection;
	static glm::vec4 FogColour;
	static float StartFogDistance;
	static float EndFogDistance;
};
