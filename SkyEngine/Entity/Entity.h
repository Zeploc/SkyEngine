// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <Box2D/Box2D.h>

// OpenGL Library Includes //

// Engine Includes //
#include <vector>

#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "Render/Meshes/Mesh.h"
#include "System/EnumTypes.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class ENGINE_API Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(std::string _FromString);

	Entity(STransform _Transform, EANCHOR _Anchor);

	//Entity() {};
	/*Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, Utils::ESHAPE _eShape);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, Utils::ESHAPE _eShape, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec2 v2FrameCounts, int _iFPS);*/
	virtual ~Entity();

	void AddMesh(TPointer<CMeshComponent> _NewMesh);

	//void AddMesh(Utils::ESHAPE _NewShape);

	virtual bool CanRender();
	virtual std::vector<TPointer<Entity>> GetAdditionalEntitiesToRender() { return {}; }
	virtual STransform GetAnchoredTransform();

	void BaseUpdate();

	virtual void Update();

	virtual bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos);

	virtual void OnDestroy();

	virtual void Reset();

	virtual void SetActive(bool _bIsActive);

	bool IsActive() const { return bActive; }

	virtual void SetVisible(bool _bIsVisible);

	bool IsVisible() const { return bVisible; }

	std::string EntityToString();

	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const TPointer<Entity>& InEntity);
	ENGINE_API friend std::istream& operator>>(std::istream& is, TPointer<Entity>& InEntity);

	void Translate(SVector _Movement);

	void Rotate(SRotator Rotate);

	void SetScale(SVector _NewScale);

	Matrix4 GetModel();

	STransform Transform;
	EANCHOR EntityAnchor;
	TPointer<CMeshComponent> EntityMesh;

	int GetEntityValue() const { return iEntityID; }

	// TODO: Remove?
	void SetInitialEntity(bool IsInitial) { bIsInitialEntity = IsInitial; }
	bool IsInitialEntity() const { return bIsInitialEntity; }

	// Box2D Physics
	b2Body* body = nullptr;

	void SetupB2BoxBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f, bool IsBullet = false);

	void SetupB2CircleBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate = true, bool bHasFixture = true, float Density = 1.0f, float Friction = 0.3f);

	void SetBox2DTransform(SVector _Position, float _Rotation);

	bool bRayCast = true;

protected:
	bool bActive = true;
	bool bVisible = true;
	bool bIsInitialEntity = false;

	int iEntityID;
};
