// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Player.h"

// Engine Includes //
#include "Engine/Camera/Camera.h"
#include "Engine/Input/Input.h"
#include "Engine/Render/Sphere.h"
#include "Engine/Sound/SoundManager.h"
#include "Engine/System/Time.h"
#include "Engine/System/Utils.h"

// Local Includes //
#include "Game/Animation/ssAnimatedModel.h"
#include "Game/Scenes/Terrain.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Engine/Entity/Button3DEntity.h"

Player::Player(glm::vec3 StartPosition) // Will also take the type of player (asthetic)
	: Entity({StartPosition, {0, 0, 0}, {0.05, 0.05, 0.05}}, EANCHOR::CENTER)
{
	std::shared_ptr<Sphere> PlayerSphere = std::make_shared<Sphere>(Sphere(2.0f, 2.0f, 2.0f, {0.9f, 0.4f, 0.7f, 1.0f}, "Resources/Images/Box.png"));
	AddMesh(PlayerSphere);
	AnimatedModel = std::make_shared<ssAnimatedModel>("Resources/Models/theDude.DAE", "Resources/Images/theDude.png");

	AnimatedModel->setCurrentAnimation(0, 30);
	AnimState = IDLE;
}

Player::~Player()
{
}

void Player::Update()
{
	Entity::Update();
	
	float MoveSpeed = 20.0f;

	if (Input::GetInstance()->KeyState[GLFW_KEY_SPACE] == Input::INPUT_FIRST_PRESS && YOffset <= 0)
	{
		//MoveSpeed = 20.0f;
		VerticalSpeed = 20.0f;
		if (AnimState != JUMP)
		{
			AnimatedModel->setCurrentAnimation(71, 80);
			AnimState = JUMP;
		}
	}

	if (YOffset > 0)
	{
		VerticalSpeed -= Gravity * Time::dTimeDelta;
	}

	YOffset += VerticalSpeed * Time::dTimeDelta;
	if (YOffset < 0)
	{
		YOffset = 0;
	}

	bool MoveForward = Input::GetInstance()->KeyState[GLFW_KEY_W] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[GLFW_KEY_W] == Input::INPUT_HOLD;
	bool MoveBackward = Input::GetInstance()->KeyState[GLFW_KEY_S] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[GLFW_KEY_S] == Input::INPUT_HOLD;
	bool MoveRight = Input::GetInstance()->KeyState[GLFW_KEY_D] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[GLFW_KEY_D] == Input::INPUT_HOLD;
	bool MoveLeft = Input::GetInstance()->KeyState[GLFW_KEY_A] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[GLFW_KEY_A] == Input::INPUT_HOLD;

	Vector2 Dir = {0, 0};

	if (MoveForward && !MoveBackward)
	{
		Dir.Y = 1;
	}
	else if (MoveBackward && !MoveForward)
	{
		Dir.Y = -1;
	}

	if (MoveRight && !MoveLeft)
	{
		Dir.X = -1;
	}
	else if (MoveLeft && !MoveRight)
	{
		Dir.X = 1;
	}

	Vector3 ForwardDirection = Camera::GetInstance()->GetCameraForwardVector();
	ForwardDirection.Y = 0;
	ForwardDirection.Normalize();
	const Vector3 ForwardMovement = ForwardDirection * (Dir.Y * static_cast<float>(Time::dTimeDelta) * MoveSpeed);

	const Vector3 RightDirection = Camera::GetInstance()->GetCameraRightVector();
	const Vector3 RightMovement = RightDirection * (Dir.X * static_cast<float>(Time::dTimeDelta) * MoveSpeed);

	// TODO: Normalise movement direction ie so diagonal is not faster 

	if (Dir.X != 0.0f || Dir.Y != 0.0f)
	{
		Vector3 DirectionVec = ForwardMovement + RightMovement;
		DirectionVec.Normalize();
		// TODO: Test pitch
		// DirectionVec.Y = -1.0f;
		DirectionVec.Normalize();
		std::cout << DirectionVec.ToString() << std::endl;
		Rotator NewRotation = DirectionVec.ToRotator();
		
		// NewRotation.Roll = 15.0f;
		std::cout << NewRotation.ToString() << std::endl;
		// NewRotation.Pitch = 0;
		// NewRotation.Roll = 0;

		// TODO: Add lerp
		Transform.Rotation = NewRotation; //  glm::rotateY(glm::vec3(0, 1, 0), angle);

		// std::shared_ptr<Scene> Scene = SceneManager::GetInstance()->GetCurrentScene();
		// std::shared_ptr<Level> FoundLevel = std::dynamic_pointer_cast<Level>(Scene);
		// if (FoundLevel)
		// {
		// 	FoundLevel->CubeButton->Transform.Rotation = Transform.Rotation;
		// }

		/*if (Dir.y < 0)
		{			
			if (AnimState != RUN_BACK && YOffset <= 0)
			{
				AnimatedModel->setCurrentAnimation(51, 70);
				AnimState = RUN_BACK;
			}
		}
		else*/
		{
			if (AnimState != RUN && YOffset <= 0)
			{
				AnimatedModel->setCurrentAnimation(31, 50);
				AnimState = RUN;
			}
		}
	}
	else
	{
		if (AnimState != IDLE && YOffset <= 0)
		{
			AnimatedModel->setCurrentAnimation(0, 30);
			AnimState = IDLE;
		}
	}

	if (TerrainRef)
	{
		Transform.Position += ForwardMovement;
		Transform.Position += RightMovement;
		Transform.Position.X = glm::clamp(Transform.Position.X, 1.0f, TerrainRef->mInfo.NumRows - 2.0f);
		Transform.Position.Z = glm::clamp(Transform.Position.Z, 1.0f, TerrainRef->mInfo.NumCols - 2.0f);

		Transform.Position.Y = TerrainRef->GetYPosition({Transform.Position.X, Transform.Position.Z}) + 0.05f + YOffset;
	}
	else
	{
		Transform.Position += RightMovement + ForwardMovement;

		Transform.Position.Y = YOffset;
	}

	glm::mat4 rotation = rotate(glm::mat4(), glm::radians(Transform.Rotation.Pitch), glm::vec3(1, 0, 0));
	rotation = rotate(rotation, glm::radians(Transform.Rotation.Yaw), glm::vec3(0, 1, 0));
	rotation = rotate(rotation, glm::radians(Transform.Rotation.Roll), glm::vec3(0, 0, 1));
	
	glm::vec3 NewCamPosition = Transform.Position.ToGLM() + glm::vec3(0, 10, -20);
	Camera::GetInstance()->SetCameraPos(NewCamPosition);
}

void Player::DrawEntity()
{	
	Entity::DrawEntity();

	// TODO: If adding mesh to this player, render error occurs related to animated model
	AnimatedModel->SetPosition(Transform.Position);
	AnimatedModel->SetScale(Transform.Scale);
	AnimatedModel->SetRotation(Transform.Rotation);
	AnimatedModel->render(TerrainRef);
}
