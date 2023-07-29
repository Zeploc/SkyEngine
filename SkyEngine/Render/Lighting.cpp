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
	glm::mat4 translate = glm::translate(glm::mat4(), ModelTransform.Position);
	glm::mat4 scale = glm::scale(glm::mat4(), ModelTransform.Scale);
	glm::mat4 rotation = rotate(glm::mat4(), glm::radians(ModelTransform.Rotation.Pitch), glm::vec3(1, 0, 0));
	rotation = rotate(rotation, glm::radians(ModelTransform.Rotation.Yaw), glm::vec3(0, 1, 0));
	rotation = rotate(rotation, glm::radians(ModelTransform.Rotation.Roll), glm::vec3(0, 0, 1));
	glm::mat4 model = translate * rotation * scale;

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(model));
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, value_ptr(LightPosition));
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, value_ptr(CameraManager::GetInstance()->GetCameraPosition()));
	glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, value_ptr(_LightInfo.LightColour));
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
