// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //
#include <Box2D/Box2D.h>

// OpenGL Library Includes //

// Engine Includes //
#include <vector>

#include "Core/SerializableVariable.h"
#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "System/EnumTypes.h"

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

class Scene;
class CComponent;

class ENGINE_API Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(std::stringstream& ss);

	Entity(STransform InTransform, std::string EntityName = {});

	//Entity() {};
	/*Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, Utils::ESHAPE _eShape);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, Utils::ESHAPE _eShape, glm::vec4 UVCoords = glm::vec4(0, 1, 0, 1));
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	Entity(FTransform _Transform, float _fWidth, float _fHeight, EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec2 v2FrameCounts, int _iFPS);*/
	virtual ~Entity();

	void AddComponent(TPointer<CComponent> NewComponent);
	void AddToScene(TPointer<Scene> InScene);
	virtual void BeginPlay();
	virtual void Unload();

	//void AddMesh(Utils::ESHAPE _NewShape);

	virtual bool CanRender();
	virtual std::vector<TPointer<Entity>> GetAdditionalEntitiesToRender() { return {}; }
	
	SVector GetForwardVector() const;
	SVector GetUpVector() const;
	SVector GetRightVector() const;

	void SetForwardVector(SVector Forward);

	/* Base Update called every frame to check before called derived Update */
	void BaseUpdate();

	virtual void Update();

	virtual bool CheckHit(SVector RayStart, SVector RayDirection, SVector& HitPos);

	virtual void OnDestroy();

	virtual void SetActive(bool _bIsActive);

	bool IsActive() const { return bActive; }

	virtual void SetVisible(bool _bIsVisible);

	bool IsVisible() const { return bVisible; }

	std::string EntityToString();
	virtual std::string GetEntityClassName();
	static TPointer<Entity> MakeEntityFromClassName(const std::string& ClassName);
	static TPointer<Entity> GetEntityFromStringStream(std::istream& is);

	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const TPointer<Entity>& InEntity);
	ENGINE_API friend std::istream& operator>>(std::istream& is, TPointer<Entity>& InEntity);

	/* Moves the position by adding on the movement*/
	void Translate(SVector _Movement);

	/* Rotates the entity by a certain amount */
	void Rotate(SRotator Rotate);

	/* Sets the entity scale */
	void SetScale(SVector _NewScale);

	Matrix4 GetModel();

	STransform Transform;

	void AssignEntityID(int ID);
	int GetEntityID() const { return EntityID; }
	std::string GetEntityName() const { return Name; }
	TPointer<Scene> GetOwningScene() const { return OwningScene; }
	TArray<TPointer<CComponent>> GetComponents() const { return Components; }
	
	TArray<SSerializableVariable> GetSerializeVariables() const { return SerializeVariables; }

	template<typename T>
	TPointer<T> FindComponent();

	bool bRayCast = true;

protected:	
	virtual void Serialize(std::ostream& os);
	void SerializeComponents(std::ostream& os);
	virtual void Deserialize(std::istream& is);
	void DeserializeComponents(std::istream& is);
	
	bool bActive = true;
	bool bVisible = true;
	std::string Name;

	TArray<TPointer<CComponent>> Components;
	TPointer<Scene> OwningScene;

	int EntityID;
	
	TArray<SSerializableVariable> SerializeVariables;
};

template <typename T>
TPointer<T> Entity::FindComponent()
{
	for (TPointer<CComponent> Component : Components)
	{
		if (TPointer<T> TestComponent = Cast<T>(Component))
		{
			return TestComponent;
		}
	}
	return nullptr;
}
