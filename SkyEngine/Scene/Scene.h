// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"

// Library Includes //

#include <Box2D/b2_world.h>

#include "Core/Asset/AssetInterface.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "System/Utils.h"

class UIText;
class Entity;

class ENGINE_API Scene : public CAssetObject
{
public:
	Scene(const std::string& InSceneName);
	virtual ~Scene() override;
	// Override shared_from_this so manual downcast not needed
	// ReSharper disable once CppHidingFunction
	std::shared_ptr<Scene> shared_from_this();
	
	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
	static std::string GetStaticName();
	std::string GetAssetClassName() override;
	void Open() override;

	void DeleteScene();

	virtual void RenderScene();

	void AddEntity(TSharedPointer<Entity> _Entity);

	void DestroyEntity(TSharedPointer<Entity> _Entity);

	int AddEntityID();
	void VerifyEntityID(TSharedPointer<Entity> EntityToVerify);

	virtual void Update();
	virtual bool OnMouseButtonPressed(int Button, int Mods);
	virtual bool OnMouseButtonReleased(int Button, int Mods);
	virtual bool OnMouseMoved(SVector2i MousePos);
	virtual bool OnMouseScrolled(float XOffset, float YOffset);
	virtual bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount);
	virtual bool OnKeyReleased(int KeyCode, int Mods);

	void OnLoaded() override;
	void OnUnloaded() override;
	virtual void BeginPlay();

	b2World& GetWorld2D() { return World2D; }

	void SetPersistantOnLoad(bool _bIsPersistant)
	{
		bIsPersistant = _bIsPersistant;
	};

	bool operator==(const Scene& rhs) const;

	std::string SceneName;
	std::vector<TSharedPointer<Entity>> Entities;
	
protected:
	
	std::vector<TSharedPointer<Entity>> DestroyedEntities;
	bool bIsPersistant = false;
	b2World World2D;

private:
	int LastEntityID = -1;
};
