// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Lighting.h"

#include "Math/Vector.h"

// Local Includes //
#include "Camera/CameraManager.h"
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
	// TODO: Store uniform locations at beginning for shader
	const uint32_t ShaderProgram = GetRenderer()->ActiveShader->GetShaderProgram();
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "LightPosition"), LightPosition);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "LightColour"), LightColour);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "LightDirection"), SunDirection);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "FogColor"), FogColour);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "StartFog"), StartFogDistance);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "EndFog"), EndFogDistance);
	GetGraphicsAPI()->PassAttributeToShader(glGetUniformLocation(ShaderProgram, "AmbientStrength"), AmbientStrength);
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
