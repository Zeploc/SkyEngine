// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Entity.h"

// Library Includes //
#include <iostream>
#include <sstream>

// Engine Includes //
#include <format>

#include "Component.h"
#include "Render/Meshes/MeshComponent.h"
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
}

Entity::Entity(STransform InTransform) : Transform(InTransform)
{
	iEntityID = Utils::AddEntityID();
	CLogManager::Get()->DisplayMessage("New Entity created with ID #" + std::to_string(iEntityID));
	Name = std::format("Entity {}", iEntityID);
}

Entity::~Entity()
{
	CLogManager::Get()->DisplayMessage("Entity " + std::to_string(iEntityID) + " destroyed!");	
}

void Entity::AddComponent(TPointer<CComponent> NewComponent)
{
	Components.push_back(NewComponent);
	NewComponent->OnAttached();
}

void Entity::AddToScene(TPointer<Scene> InScene)
{
	OwningScene = InScene;
}

void Entity::BeginPlay()
{
	for (const TPointer<CComponent>& Component : Components)
	{
		Component->BeginPlay();
	}
}

bool Entity::CanRender()
{
	if (!bVisible)
	{
		return false;
	}
	for (const TPointer<CComponent>& Component : Components)
	{
		const ISceneVisual* SceneVisual = GetInterface<ISceneVisual>(Component);
		if (SceneVisual && SceneVisual->ShouldRender())
		{
			return true;
		}
	}
	return false;
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

SVector Entity::GetForwardVector() const
{
	return Transform.Rotation.ToVector();
}

SVector Entity::GetUpVector() const
{
	// TODO: Use rotation instead for camera roll
	return {0.0f, 1.0f, 0.0f};
}

SVector Entity::GetRightVector() const
{
	return GetForwardVector().Cross(GetUpVector()).GetNormalized();
}

void Entity::SetForwardVector(SVector Forward)
{
	Transform.Rotation = Forward.ToRotator();
}

void Entity::BaseUpdate()
{
	if (!bActive)
	{
		return;
	}
	Update();
	for (const TPointer<CComponent>& Component : Components)
	{
		Component->Update();
	}
}

void Entity::Update()
{
	
}

bool Entity::CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos)
{
	for (TPointer<CComponent> Element : Components)
	{
		const TPointer<CMeshComponent> MeshComponent = Cast<CMeshComponent>(Element);
		if (MeshComponent->CheckHit(RayStart, RayDirection, HitPos, shared_from_this()))
		{
			return true;
		}
	}

	return false;
}

void Entity::OnDestroy()
{
	CLogManager::Get()->DisplayMessage("Entity with ID #" + std::to_string(iEntityID) + " destroyed!");
	for (TPointer<CComponent> Component : Components)
	{
		Component->OnDestroy();
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

void Entity::Translate(SVector _Movement)
{
	Transform.Position += _Movement;
}

void Entity::Rotate(SRotator Rotate)
{
	Transform.Rotation += Rotate;
}

void Entity::SetScale(SVector _NewScale)
{
	Transform.Scale = _NewScale;
}

Matrix4 Entity::GetModel()
{
	return Transform.GetModelMatrix();
}
