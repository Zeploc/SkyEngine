// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Core.h"

class Entity;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CComponent : public std::enable_shared_from_this<CComponent>
{
public:
	virtual ~CComponent() = default;
	CComponent(const TPointer<Entity>& InOwner);
	virtual void OnAttached();
	virtual void BeginPlay();
	virtual void Unload();

	virtual void Update();
	virtual void OnDestroy();

	TPointer<Entity> GetOwner() const { return Owner; }
	virtual std::string GetComponentClassName();

	static TPointer<CComponent> MakeComponentFromClassName(const std::string& ClassName, TPointer<Entity> Owner);
	
	ENGINE_API friend std::ostream& operator<<(std::ostream& os, const TPointer<CComponent>& InComponent);
	ENGINE_API friend std::istream& operator>>(std::istream& is, TPointer<CComponent>& InComponent);

protected:
	virtual void Serialize(std::ostream& os);
	virtual void Deserialize(std::istream& is);
	
private:
	TPointer<Entity> Owner;
	// TODO: Parent component?
	TPointer<CComponent> ParentComponent;
};
