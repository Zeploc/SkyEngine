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
#include "Engine/Math/FTransform.h"
#include "Engine/Math/Matrix.h"
#include "Engine/Math/Rotator.h"
#include "Engine/Render/Mesh.h"
#include "Engine/System/EnumTypes.h"


class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(std::string _FromString);

	Entity(FTransform _Transform, EANCHOR _Anchor);

	//Entity() {};
	/*Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, Utils::ESHAPE _eShape);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, Utils::ESHAPE _eShape, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec2 v2FrameCounts, int _iFPS);*/
	~Entity();

	void AddMesh(std::shared_ptr<Mesh> _NewMesh);

	//void AddMesh(Utils::ESHAPE _NewShape);

	virtual void DrawEntity();

	void BaseUpdate();

	virtual void Update();

	virtual void OnDestroy();

	virtual void Reset();

	virtual void SetActive(bool _bIsActive, bool _bIsInitialState = false);

	bool IsActive() const { return bActive; }

	virtual void SetVisible(bool _bIsVisible, bool _bIsInitialState = false);

	bool IsVisible() const { return bVisible; }

	std::string EntityToString();

	void Translate(Vector3 _Movement);

	void Rotate(Rotator Rotate);

	void SetScale(Vector3 _NewScale);

	Matrix4 GetModel();

	FTransform Transform;
	EANCHOR EntityAnchor;
	std::shared_ptr<Mesh> EntityMesh;

	int GetEntityValue() const { return iEntityID; }
	void SetInitialEntity(bool IsInitial) { bIsInitialEntity = IsInitial; }
	bool IsInitialEntity() const { return bIsInitialEntity; }

	// Box2D Physics
	b2Body* body = nullptr;

	void SetupB2BoxBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f, bool IsBullet = false);

	void SetupB2CircleBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f);

	void SetBox2DTransform(Vector3 _Position, float _Rotation);

	bool bRayCast = true;

protected:
	bool bActive = true;
	bool bVisible = true;
	bool bIsInitialEntity = false;

	int iEntityID;

	struct InitialState
	{
		FTransform Transform;
		EANCHOR EntityAnchor;
		bool bActive;
		bool bVisible;
		int iEntityID;
	};

	InitialState EntityInitialState;
};

#endif
