//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Player.cpp
// Description    	:    main implementation for Player
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Player.h"

// Engine Includes //
#include "Engine\Sphere.h"
#include "Engine\Input.h"
#include "Engine\Time.h"
#include "Engine\SceneManager.h"
#include "Engine\CXBOXController.h"
#include "Engine/SoundManager.h"

// Local Includes //
#include "Level.h"
#include "Terrain.h"
#include "ssAnimatedModel.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtx\string_cast.hpp>
#include <glm\gtx\rotate_vector.hpp>

#define _USE_MATH_DEFINES
#include <math.h> 

Player::Player(glm::vec3 StartPosition) // Will also take the type of player (asthetic)
	: Entity({ StartPosition , {0, 0, 0}, {0.05, 0.05, 0.05} }, Utils::CENTER)
{	
	std::shared_ptr<Sphere> PlayerSphere = std::make_shared<Sphere>(Sphere(2.0f, 2.0f, 2.0f, {0.9f, 0.4f, 0.7f, 1.0f}, "Resources/Images/Box.png"));
	AddMesh(PlayerSphere);
	AnimatedModel = std::shared_ptr<ssAnimatedModel>(new ssAnimatedModel("Resources/Models/theDude.DAE", "Resources/Images/theDude.png"));

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

	if (Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_FIRST_PRESS && YOffset <= 0)
	{
		//MoveSpeed = 20.0f;
		VerticalSpeed = 20.0f;
		if (AnimState != JUMP)
		{
			AnimatedModel->setCurrentAnimation(71, 80);
			AnimState = JUMP;
		}
	}

	if (YOffset > 0) VerticalSpeed -= Gravity * Time::dTimeDelta;

	YOffset += VerticalSpeed * Time::dTimeDelta;
	if (YOffset < 0)
	{
		YOffset = 0;
	}

	bool MoveForward = Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_FIRST_PRESS | Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_HOLD;
	bool MoveBackward = Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_FIRST_PRESS | Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_HOLD;
	bool MoveRight = Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_FIRST_PRESS | Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_HOLD;
	bool MoveLeft = Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_FIRST_PRESS | Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_HOLD;

	glm::vec2 Dir = { 0, 0 };

	if (MoveForward && !MoveBackward)
		Dir.y = 1;
	else if (MoveBackward && !MoveForward)
		Dir.y = -1;

	if (MoveRight && !MoveLeft)
		Dir.x = -1;
	else if (MoveLeft && !MoveRight)
		Dir.x = 1;
	

	glm::vec3 ForwardDirection = Camera::GetInstance()->GetCameraForwardVector();
	ForwardDirection.y = 0;
	ForwardDirection = glm::normalize(ForwardDirection);
	glm::vec3 ForwardMovement = ForwardDirection * (Dir.y * float(Time::dTimeDelta) * MoveSpeed);

	ForwardDirection = glm::rotateY(ForwardDirection, float((90.0f / 180) * M_PI));

	glm::vec3 RightMovement = ForwardDirection * (Dir.x * float(Time::dTimeDelta) * MoveSpeed);

	if (Dir.x != 0 || Dir.y != 0)
	{
		glm::vec3 DirectionVec =  ForwardMovement + RightMovement;
		glm::vec3 target = transform.Position + DirectionVec;
		glm::mat4 Rotation = glm::orientation(DirectionVec, { 0, 1, 0 });//  glm::lookAt(transform.Position, target, { 0, 1, 0 });
		glm::vec3 NewRotation = glm::vec4(1, 1, 1, 0) *  Rotation;
		NewRotation.x = glm::degrees(NewRotation.x);
		NewRotation.y = glm::degrees(NewRotation.y);
		NewRotation.z = glm::degrees(NewRotation.z);
		NewRotation -= 45;
		if (NewRotation.x < 0) NewRotation.x = 360 + NewRotation.x;
		if (NewRotation.y < 0) NewRotation.y = 360 + NewRotation.y;
		if (NewRotation.z < 0) NewRotation.z = 360 + NewRotation.z;
		std::cout << glm::to_string(NewRotation) << std::endl;
		NewRotation.x = 0;
		NewRotation.z = 0;
		transform.Rotation = NewRotation;//  glm::rotateY(glm::vec3(0, 1, 0), angle);

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
		transform.Position += ForwardMovement;
		transform.Position += RightMovement;
		transform.Position.x = glm::clamp(transform.Position.x, 1.0f, TerrainRef->mInfo.NumRows - 2.0f);
		transform.Position.z = glm::clamp(transform.Position.z, 1.0f, TerrainRef->mInfo.NumCols - 2.0f);
		
		transform.Position.y = TerrainRef->GetYPosition({ transform.Position.x, transform.Position.z }) + 0.05f + YOffset;
	}
	else
		transform.Position += RightMovement + ForwardMovement;
}

void Player::DrawEntity()
{
	Entity::DrawEntity();
	AnimatedModel->setPosition(transform.Position);
	AnimatedModel->setScale(transform.Scale);
	AnimatedModel->setRotation(transform.Rotation);
	AnimatedModel->render(TerrainRef);
}



