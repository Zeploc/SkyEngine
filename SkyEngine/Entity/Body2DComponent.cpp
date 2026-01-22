#include "SEPCH.h"
#include "Body2DComponent.h"

#include "Entity.h"
#include "Scene/Scene.h"

CCollider2DComponent::CCollider2DComponent(const TSharedPointer<Entity>& InOwner, SBodyShape InBodyShape, bool bIsStatic)
: CCollisionBodyComponent(InOwner, bIsStatic), BodyShape(InBodyShape)
{
	
}

void CCollider2DComponent::OnAttached()
{
	CCollisionBodyComponent::OnAttached();

	const TSharedPointer<Scene> Scene = GetOwner()->GetOwningScene();
	ensure(Scene != nullptr, "No owning scene found for 2d body component!");
	b2World& Box2DWorld = Scene->GetWorld2D();

	// TODO: Switch to properties
	static bool bCanRotate = true;
	static bool bIsBullet = false;
	static bool bHasFixture = false;
	static float Density = 1.0f;
	static float Friction = 0.5f;

	const STransform OwnerTransform = GetOwner()->Transform;
	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = bStaticBody ? b2_staticBody : b2_dynamicBody; // TODO: Kinematic also an option
	bodyDef.position.Set(OwnerTransform.Position.X, OwnerTransform.Position.Y);
	// TODO:
	bodyDef.userData.pointer = uintptr_t(&*this);
	body = Box2DWorld.CreateBody(&bodyDef);
	body->SetTransform(bodyDef.position, (OwnerTransform.Rotation.Roll / 180) * b2_pi);
	body->SetFixedRotation(!bCanRotate);
	bodyDef.bullet = bIsBullet;

	b2Shape* Shape = nullptr;
	
	switch (BodyShape.BodyType)
	{
	case EBodyType::Circle:
		{
			// Define another Circle shape for our dynamic body.
			b2CircleShape* circleShape = new b2CircleShape();
			circleShape->m_radius = BodyShape.Circle.Radius;
			circleShape->m_p = b2Vec2(0.0f, 0.0f);
			Shape = circleShape;
			break;
		}
	case EBodyType::Box:
		{
			// Define another box shape for our dynamic body.
			b2PolygonShape* dynamicBox = new b2PolygonShape();
			dynamicBox->SetAsBox(BodyShape.Box.BoxExtents.X, BodyShape.Box.BoxExtents.Y);
			Shape = dynamicBox;
			break;
		}
	}

	if (bHasFixture)
	{
		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = Shape;
		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = Density;
		// Override the default friction.
		fixtureDef.friction = Friction;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
	}
	else
	{
		body->CreateFixture(Shape, 0.0f);
	}
}

void CCollider2DComponent::Update()
{
	CCollisionBodyComponent::Update();
	if (body)
	{
		STransform& Transform = GetOwner()->Transform;
		// If the transforms don't match, the transform was moved manually so set the body to this instead
		if (LastBodyTransform != Transform)
		{
			body->SetTransform(b2Vec2(Transform.Position.X, Transform.Position.Y), (Transform.Rotation.Roll / 180) * b2_pi);
		}
		// Update the entity's transform to match the physics body
		else
		{
			const b2Vec2 BodyPosition = body->GetPosition();
			// For now overrides the transform of the owner since components don't have a transform
			Transform.Position = glm::vec3(BodyPosition.x, BodyPosition.y, 0.0f);
			Transform.Rotation.Roll = (body->GetAngle() / b2_pi) * 180;
		}
		LastBodyTransform = Transform;
	}
}

void CCollider2DComponent::OnDestroy()
{
	CCollisionBodyComponent::OnDestroy();	
	if (body)
	{
		body->GetWorld()->DestroyBody(body);
	}
}

void CCollider2DComponent::SetPhysicsEnabled(bool bNewPhysicsEnabled)
{
	CCollisionBodyComponent::SetPhysicsEnabled(bNewPhysicsEnabled);
	if (body)
	{
		body->SetEnabled(bNewPhysicsEnabled);
	}
}

bool CCollider2DComponent::IsPhysicsEnabled()
{
	if (!body)
	{
		return false;
	}
	return body->IsEnabled();
}

void CCollider2DComponent::Sleep()
{
	CCollisionBodyComponent::Sleep();
	if (body)
	{
		body->SetAwake(false);
	}
}

void CCollider2DComponent::Wake()
{
	CCollisionBodyComponent::Wake();
	if (body)
	{
		body->SetAwake(true);
	}
}
