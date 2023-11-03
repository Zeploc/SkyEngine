// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Lighting.h"

#include "Math/Vector.h"

// Local Includes //
#include <glew/glew.h>

#include "Core/Application.h"
#include "Render/Renderer.h"
#include "Shaders/Shader.h"

// Static variables //
SVector Lighting::LightPosition = {5, 10, 5};
SVector Lighting::LightColour = SVector(1.0f, 1.0f, 1.0f);
SVector Lighting::SunDirection = {15, 2, 25};
SVector4 Lighting::FogColour = {0.5f, 0.5f, 0.5f, 1.0f};
float Lighting::StartFogDistance = 30.0f;
float Lighting::EndFogDistance = 45.0f;
float Lighting::AmbientStrength = 0.1f;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Lighting::Lighting()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Lighting::~Lighting()
{
}

void Lighting::PassLightingToShader()
{
	TPointer<CShader> Shader = GetRenderer()->ActiveShader;
	// TODO: Store uniform locations at beginning for shader
	const uint32_t ShaderProgram = Shader->GetShaderProgram();
	TPointer<IGraphicsAPI> Api = GetGraphicsAPI();
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "LightPosition"), LightPosition);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "LightPosition"), LightPosition);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "LightColour"), LightColour);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "LightDirection"), SunDirection);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "FogColor"), FogColour);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "StartFog"), StartFogDistance);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "EndFog"), EndFogDistance);
	Api->PassAttributeToShader(Api->GetAttributeLocation(ShaderProgram, "AmbientStrength"), AmbientStrength);
}

SVector Lighting::GetLightPosition()
{
	return LightPosition;
}

SVector Lighting::GetSunDirection()
{
	return SunDirection;
}

glm::vec4 Lighting::GetFogColour()
{
	return FogColour;
}

float Lighting::GetStartFogDistance()
{
	return StartFogDistance;
}

float Lighting::GetEndFogDistance()
{
	return EndFogDistance;
}

void Lighting::SetLightPosition(SVector InLightPosition)
{
	LightPosition = InLightPosition;
}

void Lighting::SetSunDirection(SVector InSunDirection)
{
	SunDirection = InSunDirection;
}

void Lighting::SetFogColour(glm::vec4 InFogColour)
{
	FogColour = InFogColour;
}
