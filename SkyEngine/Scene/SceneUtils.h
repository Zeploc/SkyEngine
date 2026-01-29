// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Scene.h"
#include "SceneManager.h"

class SceneUtils
{
public:
	template<typename T>
	static TSharedPointer<T> FindEntityOfClass(TAssetObjectPointer<Scene> Scene = nullptr);
};

template <typename T>
TSharedPointer<T> SceneUtils::FindEntityOfClass(TAssetObjectPointer<Scene> Scene)
{
	if (!Scene)
	{
		Scene = SceneManager::GetInstance()->GetCurrentScene();
	}
	for (TSharedPointer<Entity> Entity : Scene->Entities)
	{
		TSharedPointer<T> CheckedEntity = Cast<T>(Entity);
		if (CheckedEntity)
		{
			return CheckedEntity;
		}
	}
	return nullptr;
}
