// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <vector>
#include "Particle.h"

// OpenGL Library Includes //

// Local Includes //
#include "Engine/Entity/Entity.h"

#define NUM_PARTICLES 128 * 1000

class ParticleSystemGPU : public Entity
{
public:
	ParticleSystemGPU(Utils::Transform _Transform);

	~ParticleSystemGPU();

	void Init(int ParticleCount, const char* TexturePath);

	void SetSizeRange(glm::vec2 _v2SizeRange);

	void SetSpeedRange(glm::vec2 _v2SpeedRange);

	void SetDelayRange(glm::vec2 _v2DelayRange);

	void SetFalloffRange(glm::vec2 _v2FalloffRange);

	void SetFalloffTime(glm::vec2 _v2FalloffTime);

	void SetDirectionRange(glm::vec2 _v2DirectionRangeX, glm::vec2 _v2DirectionRangeY, glm::vec2 _v2DirectionRangeZ);

	void SetPositionRange(glm::vec2 _v2StartPositionRangeX, glm::vec2 _v2StartPositionRangeY, glm::vec2 _v2StartPositionRangeZ);

	void SetGravity(float _fGravity);

	void SetLifeTime(float _fLifetime);

	static float RandomBetweenRange(float _fMin, float _fMax);

	void Update() override;

	void DrawEntity() override;

	glm::vec4 Colour;
	float ParticleSize = 0.4f;

	std::vector<glm::vec4> m_vPosition;
	std::vector<glm::vec4> m_vVelocity;

	float m_fGravity = 0.0f;
	float m_fLifeTime = 0.0f;

private:
	void BindParticleSystemGPU(const char* TexturePath);

	glm::vec2 m_v2SizeRange = {0.05, 0.2};
	glm::vec2 m_v2DelayRange = {0.1f, 0.2f};
	glm::vec2 m_v2SpeedRange = {0.7f, 4.0f};
	glm::vec2 m_v2FalloffRange = {2.0f, 5.0f};
	glm::vec2 m_v2FalloffTime = {0.5f, 1.0f};
	glm::vec2 m_v2DirectionRangeX{-0.7, 0.7};
	glm::vec2 m_v2DirectionRangeY{1, 1};
	glm::vec2 m_v2DirectionRangeZ{-0.7, 0.7};
	glm::vec2 m_v2StartPositionRangeX{0.0, 0.0};
	glm::vec2 m_v2StartPositionRangeY{0.0, 0.0};
	glm::vec2 m_v2StartPositionRangeZ{0.0, 0.0};

	float m_fCurrentLifeTime = 0;
	float m_fCurrentDelay = 0;

	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint texture;

	GLuint posVbo;
	GLuint velVbo;
	GLuint initVelVbo;

	bool bHasTexture = false;
};
