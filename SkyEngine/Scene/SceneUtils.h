// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Scene.h"
#include "SceneManager.h"

class SceneUtils
{
public:
	template<typename T>
	static THardPointer<T> FindEntityOfClass(TAssetObjectPointer<Scene> Scene = nullptr);
};

template <typename T>
THardPointer<T> SceneUtils::FindEntityOfClass(TAssetObjectPointer<Scene> Scene)
{
	if (!Scene)
	{
		Scene = SceneManager::GetInstance()->GetCurrentScene();
	}
	for (THardPointer<Entity> Entity : Scene->Entities)
	{
		THardPointer<T> CheckedEntity = Cast<T>(Entity);
		if (CheckedEntity)
		{
			return CheckedEntity;
		}
	}
	return nullptr;
}
