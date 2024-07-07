// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <Box2D/b2_body.h>

#include "CollisionBodyComponent.h"
#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Math/Vector2.h"

struct SCircleShape
{
	float Radius = 5.0f;
};
struct SBoxShape
{
	SVector2 BoxExtents = {5.0f, 5.0f};
};

enum class EBodyType
{
	Circle,
	Box
};

struct SBodyShape
{
	union
	{
		SBoxShape Box;
		SCircleShape Circle;
	};

	EBodyType BodyType;

	SBodyShape(const SBodyShape& Copy)
	{
		switch (Copy.BodyType)
		{
		case EBodyType::Circle:
			Circle = SCircleShape(Copy.Circle.Radius);
			return;
		case EBodyType::Box:
			Box = SBoxShape(Copy.Box.BoxExtents);
			return;
		}
	}
	SBodyShape(SBodyShape& Copy)
	{
		switch (Copy.BodyType)
		{
		case EBodyType::Circle:
			Circle = SCircleShape(Copy.Circle.Radius);
			return;
		case EBodyType::Box:
			Box = SBoxShape(Copy.Box.BoxExtents);
			return;
		}
	}

	SBodyShape(const float CircleRadius)
		: Circle(SCircleShape(CircleRadius)), BodyType(EBodyType::Circle)
	{
	}
	SBodyShape(const SVector2& BoxExtents)
		: Box(SBoxShape(BoxExtents)), BodyType(EBodyType::Box)
	{
	}
};

class CCollider2DComponent : public CCollisionBodyComponent
{
public:
	CCollider2DComponent(const THardPointer<Entity>& InOwner, SBodyShape InBodyShape, bool bIsStatic);

	virtual void OnAttached() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
	virtual void SetPhysicsEnabled(bool bNewPhysicsEnabled) override;
	bool IsPhysicsEnabled() override;
	void Sleep() override;
	void Wake() override;
	
	SBodyShape& GetBodyShape() { return BodyShape; }

protected:
	// Box2D Physics
	b2Body* body = nullptr;
	SBodyShape BodyShape;

	STransform LastBodyTransform;

	void SetBox2DTransform(SVector _Position, float _Rotation);
};
