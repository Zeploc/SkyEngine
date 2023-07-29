// Copyright Skyward Studios, Inc. All Rights Reserved.

#include <random>

// Library Includes //
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Engine Includes //
#include "Particle.h"
#include "Shader.h"
#include "Camera/CameraManager.h"

// This Includes //
#include "ParticleSystem.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
ParticleSystem::ParticleSystem(FTransform _Transform) : Entity(_Transform, EANCHOR::CENTER)
{
	Colour = glm::vec4(123.0f / 255.0f, 173.0f / 255.0f, 203.0f / 255.0f, 2.0f);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init(int ParticleCount, const char* TexturePath)
{
	for (int i = 0; i < ParticleCount; i++)
	{
		float Size = RandomBetweenRange(m_v2SizeRange.x, m_v2SizeRange.y);
		float Delay = RandomBetweenRange(m_v2DelayRange.x, m_v2DelayRange.y);
		float Speed = RandomBetweenRange(m_v2SpeedRange.x, m_v2SpeedRange.y);
		float Falloff = RandomBetweenRange(m_v2FalloffRange.x, m_v2FalloffRange.y);
		float FalloffTime = RandomBetweenRange(m_v2FalloffTime.x, m_v2FalloffTime.y);
		float DirectionX = RandomBetweenRange(m_v2DirectionRangeX.x, m_v2DirectionRangeX.y);
		float DirectionY = RandomBetweenRange(m_v2DirectionRangeY.x, m_v2DirectionRangeY.y);
		float DirectionZ = RandomBetweenRange(m_v2DirectionRangeZ.x, m_v2DirectionRangeZ.y);
		float PositionX = RandomBetweenRange(m_v2StartPositionRangeX.x, m_v2StartPositionRangeX.y);
		float PositionY = RandomBetweenRange(m_v2StartPositionRangeY.x, m_v2StartPositionRangeY.y);
		float PositionZ = RandomBetweenRange(m_v2StartPositionRangeZ.x, m_v2StartPositionRangeZ.y);

		Particle NewParticle = Particle(Speed, {DirectionX, DirectionY, DirectionZ}, Falloff, FalloffTime, Delay, Transform.Position + glm::vec3(PositionX, PositionY, PositionZ));
		m_vParticles.push_back(NewParticle);
	}

	BindParticleSystem(TexturePath);
}

void ParticleSystem::BindParticleSystem(const char* TexturePath)
{
	for (auto& part : m_vParticles)
	{
		m_vPosition.push_back(part.position);
	}

	program = Shader::Programs["ParticleShader"];

	texture = Shader::GetTexture(TexturePath, true);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPosition.size(), &m_vPosition[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<GLvoid*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/************************************************************
#--Description--#:	Set particle size range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new size range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetSizeRange(glm::vec2 _v2SizeRange)
{
	m_v2SizeRange = _v2SizeRange;
}

/************************************************************
#--Description--#:	Set particle speed range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new speed range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetSpeedRange(glm::vec2 _v2SpeedRange)
{
	m_v2SpeedRange = _v2SpeedRange;
}

/************************************************************
#--Description--#:	Set particle delay range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new delay range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetDelayRange(glm::vec2 _v2DelayRange)
{
	m_v2DelayRange = _v2DelayRange;
}

/************************************************************
#--Description--#:	Set particle falloff distance range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new falloff distance range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetFalloffRange(glm::vec2 _v2FalloffRange)
{
	m_v2FalloffRange = _v2FalloffRange;
}

/************************************************************
#--Description--#:	Set particle falloff time range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new falloff time range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetFalloffTime(glm::vec2 _v2FalloffTime)
{
	m_v2FalloffTime = _v2FalloffTime;
}

/************************************************************
#--Description--#:	Set particle direction range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new direction range
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetDirectionRange(glm::vec2 _v2DirectionRangeX, glm::vec2 _v2DirectionRangeY, glm::vec2 _v2DirectionRangeZ)
{
	m_v2DirectionRangeX = _v2DirectionRangeX;
	m_v2DirectionRangeY = _v2DirectionRangeY;
	m_v2DirectionRangeZ = _v2DirectionRangeZ;
}

void ParticleSystem::SetPositionRange(glm::vec2 _v2StartPositionRangeX, glm::vec2 _v2StartPositionRangeY, glm::vec2 _v2StartPositionRangeZ)
{
	m_v2StartPositionRangeX = _v2StartPositionRangeX;
	m_v2StartPositionRangeY = _v2StartPositionRangeY;
	m_v2StartPositionRangeZ = _v2StartPositionRangeZ;
}

/************************************************************
#--Description--#:	Set particle gravity
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new gravity
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetGravity(float _fGravity)
{
	m_fGravity = _fGravity;
}

/************************************************************
#--Description--#:	Set particle liftime
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new lifetime
#--Return--#: 		NA
************************************************************/
void ParticleSystem::SetLifeTime(float _fLifetime)
{
	m_fLifeTime = _fLifetime;
}

/************************************************************
#--Description--#:	Get a number between a range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	minimum and maximum range ends
#--Return--#: 		Random float within defined range
************************************************************/
float ParticleSystem::RandomBetweenRange(float _fMin, float _fMax)
{
	int fRandRangeSize = static_cast<int>(abs(_fMax - _fMin) * 100.0f);
	if (fRandRangeSize != 0)
	{
		return static_cast<float>(rand() % fRandRangeSize) / 100.0f + _fMin;
	}
	return _fMin;
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void ParticleSystem::Update()
{
	for (auto& part : m_vParticles)
	{
		part.Update();
	}
}

/************************************************************
#--Description--#:	Render to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void ParticleSystem::DrawEntity()
{
	for (int i = 0; i < m_vParticles.size(); i++)
	{
		m_vPosition[i] = m_vParticles[i].position;
	}

	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = CameraManager::GetInstance()->GetCameraForwardVector();
	vView = normalize(vView);
	vQuad1 = cross(vView, CameraManager::GetInstance()->GetCameraUpVector());
	vQuad1 = normalize(vQuad1);
	vQuad2 = cross(vView, vQuad1);
	vQuad2 = normalize(vQuad2);

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "vQuad1"), vQuad1.x, vQuad1.y,
	            vQuad1.z);
	glUniform3f(glGetUniformLocation(program, "vQuad2"), vQuad2.x, vQuad2.y,
	            vQuad2.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "vp"), 1, GL_FALSE, value_ptr(CameraManager::GetInstance()->Projection * CameraManager::GetInstance()->View));
	glUniform4f(glGetUniformLocation(program, "Colour"), Colour.r, Colour.g, Colour.b, Colour.a);
	glUniform1f(glGetUniformLocation(program, "Size"), ParticleSize);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vPosition.size(), &m_vPosition[0], GL_STATIC_DRAW);

	glDrawArrays(GL_POINTS, 0, m_vPosition.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
