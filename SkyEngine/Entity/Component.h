// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/SerializableVariable.h"

class Entity;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CComponent : public std::enable_shared_from_this<CComponent>
{
public:
	virtual ~CComponent() = default;
	CComponent(const THardPointer<Entity>& InOwner);
	virtual void OnAttached();
	virtual void BeginPlay();
	virtual void Unload();

	virtual void Update();
	virtual void OnDestroy();

	THardPointer<Entity> GetOwner() const { return Owner; }
	virtual std::string GetComponentClassName();
	TArray<SSerializableVariable> GetSerializeVariables() const { return SerializeVariables; }

	static THardPointer<CComponent> MakeComponentFromClassName(const std::string& ClassName, THardPointer<Entity> Owner);
	
	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const THardPointer<CComponent>& InComponent);
	ENGINE_API friend std::istream& operator>>(std::istream& is, THardPointer<CComponent>& InComponent);

protected:
	virtual void Serialize(std::ostream& os);
	virtual void Deserialize(std::istream& is);
	
	TArray<SSerializableVariable> SerializeVariables;
private:
	THardPointer<Entity> Owner;
	// TODO: Parent component?
	THardPointer<CComponent> ParentComponent;
	
};