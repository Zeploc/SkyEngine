// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library //
#include <glew/glew.h>

//#include <freeglut.h>

// Local Libraries //
#include "Math/Vector4.h"
#include "Math/Vector.h"

class IRenderer;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API Lighting
{
public:
	Lighting();
	~Lighting();

	static void PassLightingToShader();

	// Can't export static member variables directly if class is exported
	static SVector GetLightPosition();
	static SVector GetSunDirection();
	static glm::vec4 GetFogColour();
	static float GetStartFogDistance();
	static float GetEndFogDistance();
	static void SetLightPosition(SVector InLightPosition);
	static void SetSunDirection(SVector InSunDirection);
	static void SetFogColour(glm::vec4 InFogColour);

private:
	static SVector LightPosition;
	static SVector LightColour;
	static SVector SunDirection;
	static SVector4 FogColour;
	static float StartFogDistance;
	static float EndFogDistance;
	static float AmbientStrength;
};
