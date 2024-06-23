// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Scene.h"
#include "SceneManager.h"

class SceneUtils
{
public:
	template<typename T>
	static TPointer<T> FindEntityOfClass(TPointer<Scene> Scene = nullptr);
};

template <typename T>
TPointer<T> SceneUtils::FindEntityOfClass(TPointer<Scene> Scene)
{
	if (!Scene)
	{
		Scene = SceneManager::GetInstance()->GetCurrentScene();
	}
	for (TPointer<Entity> Entity : Scene->Entities)
	{
		TPointer<T> CheckedEntity = Cast<T>(Entity);
		if (CheckedEntity)
		{
			return CheckedEntity;
		}
	}
	return nullptr;
}
