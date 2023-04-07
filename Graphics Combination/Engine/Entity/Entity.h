// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
//Entity.h
#ifndef _ENTITY_H
#define _ENTITY_H

// Library Includes //
#include <Box2D.h>
#include <memory>

// OpenGL Library Includes //

// Engine Includes //
#include "Engine/System/Utils.h"

class Mesh;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(std::string _FromString);

	Entity(Utils::Transform _Transform, Utils::EANCHOR _Anchor);

	//Entity() {};
	/*Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, Utils::ESHAPE _eShape);
	Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, Utils::ESHAPE _eShape, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));
	Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec2 v2FrameCounts, int _iFPS);*/
	~Entity();

	void AddMesh(std::shared_ptr<Mesh> _NewMesh);

	//void AddMesh(Utils::ESHAPE _NewShape);

	virtual void DrawEntity();

	void BaseUpdate();

	virtual void Update();

	virtual void OnDestroy();

	virtual void Reset();

	virtual void SetActive(bool _bIsActive, bool _bIsInitialState = false);

	bool IsActive() { return bActive; };

	virtual void SetVisible(bool _bIsVisible, bool _bIsInitialState = false);

	bool IsVisible() { return bVisible; };

	std::string EntityToString();

	void Translate(glm::vec3 _Movement);

	void Rotate(glm::vec3 Rotate);

	void SetScale(glm::vec3 _NewScale);

	glm::mat4 GetModel();

	Utils::Transform transform;
	Utils::EANCHOR EntityAnchor;
	std::shared_ptr<Mesh> EntityMesh;

	int GetEntityValue() { return iEntityID; };
	void SetInitialEntity(bool IsInitial) { bIsInitialEntity = IsInitial; };
	bool IsInitialEntity() { return bIsInitialEntity; };

	// Box2D Physics
	b2Body* body = nullptr;

	void SetupB2BoxBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f, bool IsBullet = false);

	void SetupB2CircleBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f);

	void SetBox2DTransform(glm::vec3 _Position, float _Rotation);

	bool bRayCast = true;

protected:
	bool bActive = true;
	bool bVisible = true;
	bool bIsInitialEntity = false;

	int iEntityID;

	struct InitialState
	{
		Utils::Transform transform;
		Utils::EANCHOR EntityAnchor;
		bool bActive;
		bool bVisible;
		int iEntityID;
	};

	InitialState EntityInitialState;
};

#endif
