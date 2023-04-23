// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Engine Includes //
#include "Engine/Entity/Entity.h"
#include "Engine/Scene/SceneManager.h"

// Library Includes //
#include <vector>

// Local Includes //
#include "Game/Scenes/Level.h"

enum CurrentAnimState
{
	IDLE,
	RUN,
	RUN_BACK,
	JUMP
};

class Player : public Entity
{
public:
	Player(glm::vec3 StartPosition);

	~Player();

	void Update() override;

	void DrawEntity() override;

	int GetID()
	{
		return m_iPlayerID;
	};

	std::shared_ptr<class Terrain> TerrainRef;

	std::shared_ptr<class ssAnimatedModel> AnimatedModel;

private:
	CurrentAnimState AnimState = IDLE;

	float YOffset = 0;
	float VerticalSpeed = 0;
	float Gravity = 30.0f;
	float JumpSize = 30.0f;

	int m_iPlayerID = -1;
};
