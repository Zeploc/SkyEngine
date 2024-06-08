// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Entity.h"

// Library Includes //
#include <iostream>
#include <sstream>

// Engine Includes //
#include <format>

#include "Camera.h"
#include "Component.h"
#include "Core/StringUtils.h"
#include "Render/Meshes/MeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "System/LogManager.h"
#include "System/Utils.h"

Entity::Entity(std::stringstream& ss)
{
	std::string ConstructName;
	ss >> ConstructName;
	if (ConstructName != "[Entity]")
	{
		return;
	}
	std::shared_ptr<Entity> Entity = shared_from_this();
	ss >> Entity;
	
	SetSerializeVariable(Name);
	SetSerializeVariable(EntityID);
	// TODO: Read only (or hidden) meta data for ID
	AddMetaTag(EntityID, MetaTag_ReadOnlyEditor)
	SetSerializeVariable(Transform);
}

Entity::Entity(STransform InTransform, std::string EntityName)
: Transform(InTransform), Name(EntityName)
{
	CLogManager::Get()->DisplayMessage("New Entity created with ID #" + std::to_string(EntityID));
	if (Name.empty())
	{
		Name = std::format("Entity_{}", EntityID);
	}
	StringUtils::Replace(Name, " ", "_");
	
	SetSerializeVariable(Name);
	SetSerializeVariable(EntityID);
	AddMetaTag(EntityID, MetaTag_ReadOnlyEditor)
	SetSerializeVariable(Transform);
}

Entity::~Entity()
{
	CLogManager::Get()->DisplayMessage("Entity " + std::to_string(EntityID) + " destroyed!");	
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

void Entity::Unload()
{
	for (const TPointer<CComponent>& Component : Components)
	{
		Component->Unload();
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
		if (MeshComponent && MeshComponent->CheckHit(RayStart, RayDirection, HitPos))
		{
			return true;
		}
	}

	return false;
}

void Entity::OnDestroy()
{
	CLogManager::Get()->DisplayMessage("Entity with ID #" + std::to_string(EntityID) + " destroyed!");
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
	sEntity << this;
	return sEntity.str();
}

std::string Entity::GetEntityClassName()
{
	return "Entity";
}

TPointer<Entity> Entity::MakeEntityFromClassName(const std::string& ClassName)
{
	if (ClassName == Camera::GetStaticName())
	{
		TPointer<Camera> NewCameraEntity = std::make_shared<Camera>(STransform());
		return NewCameraEntity;
	}
	
	TPointer<Entity> NewEntity = std::make_shared<Entity>(STransform());
	return NewEntity;
}

TPointer<Entity> Entity::GetEntityFromStringStream(std::istream& is)
{	
	std::string _;
	std::string EntityClass;
	std::getline(is, _, '[');
	std::getline(is, EntityClass, ']');
	TPointer<Entity> NewEntity = MakeEntityFromClassName(EntityClass);
	is >> NewEntity;
	return NewEntity;
}

void Entity::Serialize(std::ostream& os)
{
	// std::stringstream sEntity("");
	os << "[" << GetEntityClassName() << "]";
	for (const SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		os << " " << SerializableVariable.GetSerializedVariable();
	}
}

void Entity::SerializeComponents(std::ostream& os)
{
	for (const TPointer<CComponent>& Component : Components)
	{
		os << Component;
	}
}

void Entity::Deserialize(std::istream& is)
{
	for (SSerializableVariable& SerializableVariable : SerializeVariables)
	{
		SerializableVariable.SetDeserializedVariable(is);
	}
}

void Entity::DeserializeComponents(std::istream& is)
{
	while (is.peek() != EOF)
	{
		const std::istream::pos_type OriginalPosition = is.tellg();	
		std::string ComponentType;
		std::string _;
		std::getline(is, _, '[');
		std::getline(is, ComponentType, ']');
		// is >> ComponentType;
		TPointer<CComponent> NewComponent = CComponent::MakeComponentFromClassName(ComponentType, shared_from_this());
		if (NewComponent)
		{
			is >> NewComponent;
			AddComponent(NewComponent);
			continue;
		}
		is.seekg(OriginalPosition);
		break;
	}
}

std::ostream& operator<<(std::ostream& os, const TPointer<Entity>& InEntity)
{
	InEntity->Serialize(os);
	InEntity->SerializeComponents(os);
	return os;
}

std::istream& operator>>(std::istream& is, TPointer<Entity>& InEntity)
{
	InEntity->Deserialize(is);
	InEntity->DeserializeComponents(is);
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

void Entity::AssignEntityID(int ID)
{
	EntityID = ID;
}
