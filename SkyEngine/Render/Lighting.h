// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// OpenGL Library //
#include <glew/glew.h>

//#include <freeglut.h>

// Local Libraries //
#include "Math/Vector4.h"
#include "Math/Vector.h"

class IGraphicsInstance;

class ENGINE_API Lighting
{
public:
	Lighting();
	~Lighting();

	static void PassLightingToShader(const TPointer<IGraphicsInstance>& GraphicsInstance);

	// Can't export static member variables directly if class is exported
	static inline SVector GetLightPosition();
	static inline SVector GetSunDirection();
	static inline glm::vec4 GetFogColour();
	static inline float GetStartFogDistance();
	static inline float GetEndFogDistance();
	static inline void SetLightPosition(SVector InLightPosition);
	static inline void SetSunDirection(SVector InSunDirection);
	static inline void SetFogColour(glm::vec4 InFogColour);

private:
	static SVector LightPosition;
	static SVector LightColour;
	static SVector SunDirection;
	static SVector4 FogColour;
	static float StartFogDistance;
	static float EndFogDistance;
	static float AmbientStrength;
};
