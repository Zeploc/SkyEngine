// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "ParticleSystemGPU.h"

// Engine Includes //
#include "Particle.h"
#include "Shader.h"
#include "Engine/Camera/Camera.h"

// Library Includes //
#include <random>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
ParticleSystemGPU::ParticleSystemGPU(FTransform _Transform) : Entity(_Transform, EANCHOR::CENTER)
{
	Colour = glm::vec4(123.0f / 255.0f, 173.0f / 255.0f, 203.0f / 255.0f, 2.0f);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
ParticleSystemGPU::~ParticleSystemGPU()
{
}

void ParticleSystemGPU::Init(int ParticleCount, const char* TexturePath)
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

		Particle NewParticle = Particle(Speed, {DirectionX, DirectionY, DirectionZ}, Falloff, FalloffTime, Delay, Transform.Position.ToGLM() + glm::vec3(PositionX, PositionY, PositionZ));
		//m_vParticles.push_back(NewParticle);
		m_vPosition.push_back(glm::vec4(Transform.Position.ToGLM() + glm::vec3(PositionX, PositionY, PositionZ), FalloffTime));
		m_vVelocity.push_back(glm::vec4(glm::vec3(DirectionX, DirectionY, DirectionZ) * Speed, 0.0f));
	}

	BindParticleSystemGPU(TexturePath);
}

void ParticleSystemGPU::BindParticleSystemGPU(const char* TexturePath)
{
	/*for (auto& part : m_vParticles)
	{
		m_vPosition.push_back(part.position);
	}*/

	program = Shader::Programs["GPUParticlesProgram"];

	texture = Shader::GetTexture(TexturePath, true);

	// Position
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_vPosition.size(), &m_vPosition[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posVbo);

	// Velocity
	glGenBuffers(1, &velVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_vVelocity.size(), &m_vVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velVbo);
	// Initial Velocity
	glGenBuffers(1, &initVelVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initVelVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * m_vVelocity.size(), &m_vVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, initVelVbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// bind pos buffer
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/************************************************************
#--Description--#:	Set particle size range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new size range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetSizeRange(glm::vec2 _v2SizeRange)
{
	m_v2SizeRange = _v2SizeRange;
}

/************************************************************
#--Description--#:	Set particle speed range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new speed range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetSpeedRange(glm::vec2 _v2SpeedRange)
{
	m_v2SpeedRange = _v2SpeedRange;
}

/************************************************************
#--Description--#:	Set particle delay range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new delay range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetDelayRange(glm::vec2 _v2DelayRange)
{
	m_v2DelayRange = _v2DelayRange;
}

/************************************************************
#--Description--#:	Set particle falloff distance range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new falloff distance range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetFalloffRange(glm::vec2 _v2FalloffRange)
{
	m_v2FalloffRange = _v2FalloffRange;
}

/************************************************************
#--Description--#:	Set particle falloff time range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new falloff time range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetFalloffTime(glm::vec2 _v2FalloffTime)
{
	m_v2FalloffTime = _v2FalloffTime;
}

/************************************************************
#--Description--#:	Set particle direction range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new direction range
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetDirectionRange(glm::vec2 _v2DirectionRangeX, glm::vec2 _v2DirectionRangeY, glm::vec2 _v2DirectionRangeZ)
{
	m_v2DirectionRangeX = _v2DirectionRangeX;
	m_v2DirectionRangeY = _v2DirectionRangeY;
	m_v2DirectionRangeZ = _v2DirectionRangeZ;
}

void ParticleSystemGPU::SetPositionRange(glm::vec2 _v2StartPositionRangeX, glm::vec2 _v2StartPositionRangeY, glm::vec2 _v2StartPositionRangeZ)
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
void ParticleSystemGPU::SetGravity(float _fGravity)
{
	m_fGravity = _fGravity;
}

/************************************************************
#--Description--#:	Set particle liftime
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Vector 2 new lifetime
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::SetLifeTime(float _fLifetime)
{
	m_fLifeTime = _fLifetime;
}

/************************************************************
#--Description--#:	Get a number between a range
#--Author--#: 		Alex Coultas
#--Parameters--#: 	minimum and maximum range ends
#--Return--#: 		Random float within defined range
************************************************************/
float ParticleSystemGPU::RandomBetweenRange(float _fMin, float _fMax)
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
void ParticleSystemGPU::Update()
{
	/*for (auto& part : m_vParticles)
	{
		part.Update();
	}

	std::cout << glm::to_string(m_vParticles[0].position) << std::endl;*/
}

/************************************************************
#--Description--#:	Render to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void ParticleSystemGPU::DrawEntity()
{
	glUseProgram(Shader::Programs["ComputeProgram"]);

	glDispatchCompute(NUM_PARTICLES / 128, 1, 1);
	// Sync, wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//glm::mat4 viewMat = Camera::GetInstance()->view;
	//glm::vec3 vQuad1, vQuad2;
	//glm::vec3 vView = Camera::GetInstance()->GetCameraForwardVector();
	//vView = glm::normalize(vView);
	//vQuad1 = glm::cross(vView, Camera::GetInstance()->GetCameraUpVector());
	//vQuad1 = glm::normalize(vQuad1);
	//vQuad2 = glm::cross(vView, vQuad1);
	//vQuad2 = glm::normalize(vQuad2);
	//	
	////return;
	glUseProgram(program);
	///*glUniform3f(glGetUniformLocation(program, "vQuad1"), vQuad1.x, vQuad1.y,
	//	vQuad1.z);
	//glUniform3f(glGetUniformLocation(program, "vQuad2"), vQuad2.x, vQuad2.y,
	//	vQuad2.z);*/

	glUniformMatrix4fv(glGetUniformLocation(program, "vp"), 1, GL_FALSE, value_ptr(Camera::GetInstance()->Projection.ToGLM() * Camera::GetInstance()->View.ToGLM()));
	////glUniform4f(glGetUniformLocation(program, "Colour"), Colour.r, Colour.g, Colour.b, Colour.a);
	////glUniform1f(glGetUniformLocation(program, "Size"), ParticleSize);

	///*glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(program, "Texture"), 0);*/

	//glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glDepthMask(GL_FALSE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, nullptr);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	////glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
	//glDepthMask(GL_TRUE);
}
