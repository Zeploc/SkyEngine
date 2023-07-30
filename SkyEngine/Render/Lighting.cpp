// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Lighting.h"

#include "Math/Vector.h"

// OpenGL Includes //
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local Includes //
#include "Camera/CameraManager.h"

// Static variables //
Vector3 Lighting::LightPosition = {5, 10, 5};
Vector3 Lighting::SunDirection = {15, 2, 25};
glm::vec4 Lighting::FogColour = {0.5f, 0.5f, 0.5f, 1.0f};
float Lighting::StartFogDistance = 30.0f;
float Lighting::EndFogDistance = 45.0f;

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

void Lighting::PassLightingToShader(GLuint program, LightInfo _LightInfo, FTransform ModelTransform)
{
	glm::mat4 model = ModelTransform.GetModelMatrix();

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(model));
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, LightPosition.ToValuePtr());
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, CameraManager::GetInstance()->GetCameraPosition().ToValuePtr());
	glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, _LightInfo.LightColour.ToValuePtr());
	glUniform1f(glGetUniformLocation(program, "ambientStr"), _LightInfo.fAmbientStrength);
	glUniform1f(glGetUniformLocation(program, "lightSpecStr"), _LightInfo.fLightSpecStrength);
	glUniform1f(glGetUniformLocation(program, "shininess"), _LightInfo.fShininess);
}

Vector3 Lighting::GetLightPosition()
{
	return LightPosition;
}

Vector3 Lighting::GetSunDirection()
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

void Lighting::SetLightPosition(Vector3 InLightPosition)
{
	LightPosition = InLightPosition;
}

void Lighting::SetSunDirection(Vector3 InSunDirection)
{
	SunDirection = InSunDirection;
}

void Lighting::SetFogColour(glm::vec4 InFogColour)
{
	FogColour = InFogColour;
}
