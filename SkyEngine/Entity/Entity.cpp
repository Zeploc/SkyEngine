// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Entity.h"

// Library Includes //
#include <iostream>
#include <sstream>

// Engine Includes //
#include "CollisionBounds.h"
#include "Render/Meshes/Mesh.h"
#include "System/LogManager.h"
#include "System/Utils.h"

Entity::Entity(std::string _FromString)
{
	std::stringstream ss(_FromString);
	std::string Name;
	ss >> Name;
	if (Name != "[Entity]")
	{
		return;
	}
	ss >> iEntityID;
	ss >> Transform;

	EntityAnchor = EANCHOR::CENTER;
}

//************************************************************
//#--Description--#:	Constructor function with base position
//#--Author--#: 		Alex Coultas
//#--Parameters--#:		Takes contructor values
//#--Return--#: 		NA
//************************************************************/
Entity::Entity(STransform _Transform, EANCHOR _Anchor) : Transform(_Transform), EntityAnchor(_Anchor)
{
	iEntityID = Utils::AddEntityID();
	LogManager::GetInstance()->DisplayLogMessage("New Entity created with ID #" + std::to_string(iEntityID));
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Entity::~Entity()
{
	EntityMesh = nullptr;
	LogManager::GetInstance()->DisplayLogMessage("Entity " + std::to_string(iEntityID) + " destroyed!");	
}

void Entity::AddMesh(TPointer<CMeshComponent> _NewMesh)
{
	EntityMesh = _NewMesh;
	// TODO: Currently binds here when added to entity, should restructure this
	EntityMesh->BindMeshData();
}

bool Entity::CanRender()
{
	if (!EntityMesh || !bVisible)
	{
		return false;
	}
	return true;
}

//void Entity::AddMesh(Utils::ESHAPE _NewShape)
//{
//	switch (_NewShape)
//	{
//	case Utils::PLANE:
//		EntityMesh = std::make_shared<Plane>(Plane(1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	case Utils::PYRAMID:
//		EntityMesh = std::make_shared<Pyramid>(Pyramid(1, 1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	case EMESHTYPE::CUBE:
//		break;
//	case Utils::SPHERE:
//		EntityMesh = std::make_shared<Sphere>(Sphere(1, 1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	default:
//		break;
//	}
//	EntityMesh->EntityRef = this->shared_from_this();
//}

STransform Entity::GetAnchoredTransform()
{
	STransform AnchoredTransform = Transform;
	if (EntityMesh->GetCollisionBounds())
	{
		AnchoredTransform.Position = Utils::GetAncoredPosition(Transform.Position, EntityMesh->GetCollisionBounds()->GetDimensions(), EntityAnchor);
	}
	else
	{
		AnchoredTransform.Position = Utils::GetAncoredPosition(Transform.Position, glm::vec3(EntityMesh->m_fWidth, EntityMesh->m_fHeight, EntityMesh->m_fDepth), EntityAnchor);
	}
	return AnchoredTransform;
}

/************************************************************
#--Description--#: 	Base Update called every frame to check before called derived Update
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::BaseUpdate()
{
	if (!bActive)
	{
		return;
	}
	Update();
	if (EntityMesh)
	{
		EntityMesh->Update();
	}
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::Update()
{
	if (body)
	{
		b2Vec2 BodyPosition = body->GetPosition();
		Transform.Position = glm::vec3(BodyPosition.x, BodyPosition.y, 0.0f);
		Transform.Rotation.Roll = (body->GetAngle() / b2_pi) * 180;
	}
}

bool Entity::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos)
{
	if (!EntityMesh)
	{
		return false;
	}

	return EntityMesh->CheckHit(RayStart, RayDirection, HitPos, shared_from_this());
}

/************************************************************
#--Description--#: 	Virtual function ran when the entity is destroyed
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::OnDestroy()
{
	LogManager::GetInstance()->DisplayLogMessage("Entity with ID #" + std::to_string(iEntityID) + " destroyed!");
	if (body)
	{
		body->SetEnabled(false);
	}
}

void Entity::Reset()
{
	// Reset all entity variables
	if (body)
	{
		body->SetTransform(b2Vec2(Transform.Position.X, Transform.Position.Y), (Transform.Rotation.Roll / 180) * b2_pi);
		body->SetAwake(true);
		body->SetEnabled(bActive);
		body->SetLinearVelocity(b2Vec2_zero);
		body->SetAngularVelocity(0.0f);
	}
	// Reset Entity Mesh
	if (EntityMesh)
	{
		EntityMesh->Reset();
	}
}

void Entity::SetActive(bool _bIsActive)
{
	bActive = _bIsActive;
}

void Entity::SetVisible(bool _bIsVisible)
{
	bVisible = _bIsVisible;
}

std::string Entity::EntityToString()
{
	std::stringstream sEntity("");
	sEntity << "[Entity] ";
	sEntity << iEntityID << " ";
	sEntity << Transform.ToString();
	return sEntity.str();
}

std::ostream& operator<<(std::ostream& os, const TPointer<Entity>& InEntity)
{
	// std::stringstream sEntity("");
	os << "[Entity] ";
	os << InEntity->iEntityID << " ";
	os << InEntity->Transform;
	return os;
}

std::istream& operator>>(std::istream& is, TPointer<Entity>& InEntity)
{
	std::string Empty;
	// TODO: Remove need for first space removal
	std::getline(is, Empty, ' ');
	is >> InEntity->iEntityID;
	// std::getline(is, Empty, ' ');
	is >> InEntity->Transform;
	return is;
}

/************************************************************
#--Description--#: 	Moves the position by adding on the movement
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the movement to move the entity by
#--Return--#: 		NA
************************************************************/
void Entity::Translate(SVector _Movement)
{
	Transform.Position += _Movement;
}

/************************************************************
#--Description--#: 	Rotates the entity by a certain amount
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the rotation to add
#--Return--#: 		NA
************************************************************/
void Entity::Rotate(SRotator Rotate)
{
	Transform.Rotation += Rotate;
}

/************************************************************
#--Description--#: 	Sets the entity scale
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the new scale
#--Return--#: 		NA
************************************************************/
void Entity::SetScale(SVector _NewScale)
{
	Transform.Scale = _NewScale;
}

Matrix4 Entity::GetModel()
{
	return Transform.GetModelMatrix();
}

void Entity::SetupB2BoxBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate, bool bHasFixture, float Density, float Friction, bool IsBullet)
{
	if (EntityMesh)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = BodyType;
		bodyDef.position.Set(Transform.Position.X, Transform.Position.Y);
		// TODO:
		bodyDef.userData.pointer = uintptr_t(&*this);
		body = Box2DWorld.CreateBody(&bodyDef);
		body->SetTransform(bodyDef.position, (Transform.Rotation.Roll / 180) * b2_pi);
		body->SetFixedRotation(!bCanRotate);
		bodyDef.bullet = true;

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(EntityMesh->m_fWidth / 2.0f, EntityMesh->m_fHeight / 2.0f);
		if (bHasFixture)
		{
			// Define the dynamic body fixture.
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			// Set the box density to be non-zero, so it will be dynamic.
			fixtureDef.density = Density;
			// Override the default friction.
			fixtureDef.friction = Friction;

			// Add the shape to the body.
			body->CreateFixture(&fixtureDef);
		}
		else
		{
			body->CreateFixture(&dynamicBox, 0.0f);
		}
	}
	else
	{
		LogManager::GetInstance()->DisplayLogMessage("Failed to add Box2D body to Entity #" + std::to_string(iEntityID) + ", Entity has no Mesh");
	}
}

void Entity::SetupB2CircleBody(b2World& Box2DWorld, b2BodyType BodyType, bool bCanRotate, bool bHasFixture, float Density, float Friction)
{
	if (EntityMesh)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = BodyType;
		bodyDef.position.Set(Transform.Position.X, Transform.Position.Y);
		bodyDef.userData.pointer = uintptr_t(&*this);
		body = Box2DWorld.CreateBody(&bodyDef);
		body->SetTransform(bodyDef.position, (Transform.Rotation.Roll / 180) * b2_pi);
		body->SetFixedRotation(!bCanRotate);

		// Define another Circle shape for our dynamic body.
		b2CircleShape circleShape;
		circleShape.m_radius = EntityMesh->m_fHeight / 2.0f;
		circleShape.m_p = b2Vec2(0.0f, 0.0f);

		if (bHasFixture)
		{
			// Define the dynamic body fixture.
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			// Set the box density to be non-zero, so it will be dynamic.
			fixtureDef.density = Density;
			// Override the default friction.
			fixtureDef.friction = Friction;

			// Add the shape to the body.
			body->CreateFixture(&fixtureDef);
		}
		else
		{
			body->CreateFixture(&circleShape, 0.0f);
		}
	}
	else
	{
		LogManager::GetInstance()->DisplayLogMessage("Failed to add Box2D body to Entity #" + std::to_string(iEntityID) + ", Entity has no Mesh");
	}
}

void Entity::SetBox2DTransform(SVector _Position, float _Rotation)
{
	if (body)
	{
		body->SetTransform(b2Vec2(_Position.X, _Position.Y), (_Rotation / 180) * b2_pi);
	}
}
